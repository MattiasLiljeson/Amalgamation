#include "PhysicsSystem.h"
#include "Transform.h"
#include "BodyInitData.h"
#include <PhysicsController.h>
#include "PhysicsBody.h"
#include "RenderInfo.h"
#include "ConnectionPointSet.h"
#include "ShipFlyController.h"
#include "ShipModule.h"
#include "Connector1to2Module.h"
#include "NetworkSynced.h"

PhysicsSystem::PhysicsSystem()
	: EntitySystem(SystemType::PhysicsSystem, 2, ComponentType::Transform, ComponentType::PhysicsBody)
{
	m_physicsController = new PhysicsController();
}


PhysicsSystem::~PhysicsSystem()
{
	delete m_physicsController;
}

void PhysicsSystem::initialize()
{
}

void PhysicsSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();
	m_physicsController->Update(dt);

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		PhysicsBody* body =
			static_cast<PhysicsBody*>(
			m_world->getComponentManager()->getComponent( p_entities[i],
			ComponentType::getTypeFor(ComponentType::PhysicsBody)));

		if (body->m_id == -1)
		{
			//Not initialized! Initialize the physics component of the entity
			initializeEntity(p_entities[i]);
		}
		else
		{
			// Get rigidbody from id
			Body* b = m_physicsController->getBody(body->m_id);

			// Handle parenting
			handleCompoundBodyDependencies(p_entities[i]);

			// Update the rigidbody
			if (!b->IsStatic())
			{
				AglMatrix world = b->GetWorld();
				Transform* t = static_cast<Transform*>( p_entities[i]->getComponent(
					ComponentType::Transform));
				AglVector3 pos;
				AglVector3 scale;
				AglQuaternion rot;
				AglMatrix::matrixToComponents(world, scale, rot, pos);
				t->setTranslation(pos);
				t->setRotation(rot);
				t->setScale(scale);

				if (!b->IsCompoundBody())
				{
					if (((RigidBody*)b)->GetType() == BOX)
					{
						RigidBodyBox* box = (RigidBodyBox*)b;
						t->setScale(box->GetSizeAsVector3()*0.5f);
					}
				}
			}

		}

		//Check if the object is a ship
		NetworkSynced* ns = static_cast<NetworkSynced*>(p_entities[i]->getComponent(ComponentType::NetworkSynced));
		if (ns && ns->getNetworkType() == EntityType::Ship)
		{
			queryShipCollision(p_entities[i], p_entities);
		}
	}
}

void PhysicsSystem::applyImpulse(int p_bodyID, AglVector3 p_impulse, AglVector3 p_angularImpulse)
{
	m_physicsController->ApplyExternalImpulse(p_bodyID, p_impulse, p_angularImpulse);
}

void PhysicsSystem::initializeEntity(Entity* p_entity)
{
	PhysicsBody* body =
		static_cast<PhysicsBody*>(
		m_world->getComponentManager()->getComponent( p_entity,
		ComponentType::getTypeFor(ComponentType::PhysicsBody)));

	BodyInitData* init =
		static_cast<BodyInitData*>(
		m_world->getComponentManager()->getComponent( p_entity,
		ComponentType::getTypeFor(ComponentType::BodyInitData)));

	if (init)
	{
		CompoundBody* cb = NULL;
		if (body->getParentId() >= 0)
			cb = static_cast<CompoundBody*>(m_physicsController->getBody(body->getParentId()));

		int t=0;
		int* bodyId = &t; // temp storage of id
		AglVector3 offset=AglVector3(0.0f,0.0f,0.0f);
		if (init->m_compound)
		{
			// Add compound body as id to component
			body->m_id = m_physicsController->AddCompoundBody(init->m_position);
			cb = static_cast<CompoundBody*>(m_physicsController->getBody(body->m_id));
			offset = init->m_position;
		}
		else // repoint id storage; only add shape id to body component if not compound
			bodyId = &(body->m_id); 
		
		// Add shape
		if (init->m_type == BodyInitData::BOX)
		{
			AglMatrix world;
			AglMatrix::componentsToMatrix(world, AglVector3::one(), init->m_orientation, init->m_position-offset);
			*bodyId = m_physicsController->AddBox(world,
				init->m_scale*2, 1, 
				init->m_velocity, 
				init->m_angularVelocity, 
				init->m_static,
				cb, init->m_impulseEnabled, init->m_collisionEnabled);
		}
		else if (init->m_type == BodyInitData::SPHERE)
		{
			float radius = max(max(init->m_scale.x, init->m_scale.y), init->m_scale.z);
			AglMatrix world;
			AglMatrix::componentsToMatrix(world, AglVector3::one(), init->m_orientation, init->m_position-offset);
			*bodyId = m_physicsController->AddSphere(world,
				radius, 1, 
				init->m_velocity, 
				init->m_angularVelocity, 
				init->m_static,
				cb, init->m_impulseEnabled, init->m_collisionEnabled);
		}
		else if (init->m_type == BodyInitData::MESH)
		{
			//Not supported
			//m_physicsController->AddMeshBody()
		}
		else
		{
			//Not Supported
		}

		//Add the physics body to the entity map
		while (body->m_id >= m_entityMap.size())
			m_entityMap.push_back(-1);

		m_entityMap[body->m_id] = p_entity->getIndex();
		
		// remove settings component
		p_entity->removeComponent(ComponentType::BodyInitData);
		p_entity->applyComponentChanges();
	}
	else // fallback settings
	{
		Transform* t =
			static_cast<Transform*>(
			m_world->getComponentManager()->getComponent( p_entity,
			ComponentType::getTypeFor(ComponentType::Transform)));
		body->m_id = m_physicsController->AddBox(t->getTranslation(), t->getScale()*2, 1, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
	}
}

void PhysicsSystem::handleCompoundBodyDependencies(Entity* p_entity)
{
	ShipModule* module = static_cast<ShipModule*>(
		p_entity->getComponent(ComponentType::ShipModule));
	if (module)
	{
		PhysicsBody* body = static_cast<PhysicsBody*>(p_entity->getComponent(ComponentType::PhysicsBody));
		if (module->m_parentEntity < 0 && body->getParentId() >= 0)
		{
			//Remove dependency in physics

			RigidBody* rb = (RigidBody*)m_physicsController->getBody(body->m_id);

			m_physicsController->DetachBodyFromCompound(rb);
			body->unspecifyParent();
			return;
		}
	}

	/*if (p_bodyComponent->isParentChanged())
	{
		// First, retrieve the ids
		int oldId = p_bodyComponent->getOldParentId();
		int newId = p_bodyComponent->getParentId();

		// Then the pointers to the bodies
		Body* oldparent = NULL;
		Body* newparent = NULL;
		RigidBody* thisBody = static_cast<RigidBody*>(p_rigidBody);

		if (oldId!=-1)
			oldparent = m_physicsController->getBody(p_bodyComponent->getOldParentId());

		if (newId!=-1)
			newparent = m_physicsController->getBody(p_bodyComponent->getParentId());

		// Detach this body from old parent
		if (oldparent!=NULL && oldparent->IsCompoundBody())
		{
			static_cast<CompoundBody*>(oldparent)->DetachChild(thisBody);
		}

		// Attach this body to new parent
		if (newparent!=NULL && newparent->IsCompoundBody())
		{
			static_cast<CompoundBody*>(newparent)->AddChild(thisBody);
		}

		// Reset dirtybit
		p_bodyComponent->resetParentChangedStatus();
	}*/
}

void PhysicsSystem::addModulesToShip(PhysicsBody* p_body, AglVector3 p_position)
{
	EntitySystem* tempSys = NULL;
/*
	// Load cube model used as graphic representation for all "graphical" entities.
	tempSys = m_world->getSystem(SystemType::GraphicsBackendSystem);
	GraphicsBackendSystem* graphicsBackend = static_cast<GraphicsBackendSystem*>(tempSys);
	int cubeMeshId = graphicsBackend->createMesh( "P_cube" );

	//Create modules that attaches to the ship
	Entity* entity = m_world->createEntity();
	int shipId = entity->getIndex();
	Component* component = new RenderInfo( cubeMeshId );
	entity->addComponent( ComponentType::RenderInfo, component );
	component = new Transform(p_position.x, p_position.y, p_position.z);
	entity->addComponent( ComponentType::Transform, component );

	PhysicsBody* body = new PhysicsBody();
	body->setParentId(p_body->m_id);

	entity->addComponent( ComponentType::PhysicsBody, 
		body);

	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(p_position,
		AglQuaternion::identity(),
		AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE));

	m_world->addEntity(entity);
	*/
}

void PhysicsSystem::queryShipCollision(Entity* ship, const vector<Entity*>& p_others)
{
	return;
	static int counter=0;

	ConnectionPointSet* cps = static_cast<ConnectionPointSet*>(
		m_world->getComponentManager()->getComponent( ship,
		ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

	if (!cps)
		return;

	vector<pair<ConnectionPoint*, Entity*>> connectionPoints = getFreeConnectionPoints(cps, ship);

	if (connectionPoints.size() == 0)
		return;

	int curr = 0;


	PhysicsBody* body =
		static_cast<PhysicsBody*>(
		m_world->getComponentManager()->getComponent( ship,
		ComponentType::getTypeFor(ComponentType::PhysicsBody)));

	vector<unsigned int> collisions = m_physicsController->CollidesWith(body->m_id);
	if (collisions.size() > 0)
	{
		for (unsigned int i = 0; i < p_others.size(); i++)
		{
			PhysicsBody* PhysModule =
				static_cast<PhysicsBody*>(
				m_world->getComponentManager()->getComponent(p_others[i],
				ComponentType::getTypeFor(ComponentType::PhysicsBody)));

			ShipModule* module = static_cast<ShipModule*>(
				m_world->getComponentManager()->getComponent(p_others[i],
				ComponentType::getTypeFor(ComponentType::ShipModule)));

			if (module && module->m_parentEntity < 0)
			{
				for (unsigned int j = 0; j < collisions.size(); j++)
				{
					if (collisions[j] == PhysModule->m_id)
					{
						CompoundBody* comp = (CompoundBody*)m_physicsController->getBody(body->m_id);
						RigidBody* r = (RigidBody*)m_physicsController->getBody(PhysModule->m_id);



						AglMatrix transform = offset(connectionPoints[curr].second, connectionPoints[curr].first->cpTransform);
						m_physicsController->AttachBodyToCompound(comp, r, transform);
						connectionPoints[curr].first->cpConnectedEntity = p_others[i]->getIndex();
						
						module->m_parentEntity = connectionPoints[curr].second->getIndex();

						PhysModule->setParentId(body->m_id);

						//Kör en return just nu. Kan ändras sen. Tror inte det blir problem
						return;
						curr++;
						if (curr >= connectionPoints.size())
							return;
					}
				}
			}
		}
	}
}
vector<pair<ConnectionPoint*, Entity*>> PhysicsSystem::getFreeConnectionPoints(ConnectionPointSet* p_set, Entity* p_parent){
	vector<pair<ConnectionPoint*, Entity*>> free;
	for (unsigned int i = 0; i < p_set->m_connectionPoints.size(); i++)
	{
		if (p_set->m_connectionPoints[i].cpConnectedEntity < 0)
			free.push_back(pair<ConnectionPoint*, Entity*>(
				&p_set->m_connectionPoints[i], p_parent));
		else
		{
			Entity* module = m_world->getEntity(p_set->m_connectionPoints[i].cpConnectedEntity);

			ConnectionPointSet* connector =
				static_cast<ConnectionPointSet*>(
				m_world->getComponentManager()->getComponent(module,
				ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));
			if (connector)
			{
				vector<pair<ConnectionPoint*, Entity*>> moreFree 
					= getFreeConnectionPoints(connector, module);
				for (unsigned int j = 0; j < moreFree.size(); j++)
					free.push_back(moreFree[j]);
			}
		}
	}
	return free;
}
AglMatrix PhysicsSystem::offset(Entity* p_entity, AglMatrix p_base)
{
	AglMatrix transform = p_base;
	ShipModule* module = static_cast<ShipModule*>(p_entity->getComponent(ComponentType::ShipModule));
	while (module)
	{
		Entity* parent = m_world->getEntity(module->m_parentEntity);

		ConnectionPointSet* cps = static_cast<ConnectionPointSet*>(
			m_world->getComponentManager()->getComponent(parent,
			ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));

		unsigned int ind = 0;
		for (unsigned int i = 1; i < cps->m_connectionPoints.size(); i++)
		{
			if (cps->m_connectionPoints[i].cpConnectedEntity == p_entity->getIndex())
				ind = i;
		}

		transform = transform * cps->m_connectionPoints[ind].cpTransform;
		module = static_cast<ShipModule*>(parent->getComponent(ComponentType::ShipModule));
		p_entity = parent;
	}
	return transform;
}
PhysicsController* PhysicsSystem::getPhysicsController()
{
	return m_physicsController;
}
