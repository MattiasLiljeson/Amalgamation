#include "LookAtSystem.h"

#include "CameraInfo.h"
#include "LookAtEntity.h"
#include "Transform.h"
#include "GameplayTags.h"
#include "ValueClamp.h"
#include "PhysicsBody.h"
#include "MeshOffsetTransform.h"
#include "ShipManagerSystem.h"
#include "EntityUpdatePacket.h"
#include "..\..\NetComm\Src\Packet.h"
#include "PacketType.h"
#include "NetworkSynced.h"
#include "ServerStateSystem.h"

LookAtSystem::LookAtSystem(TcpServer* p_server) : 
EntitySystem( SystemType::LookAtSystem, 2,
			 ComponentType::ComponentTypeIdx::LookAtEntity,
			 ComponentType::ComponentTypeIdx::Transform)
{
	m_server = p_server;
}


LookAtSystem::~LookAtSystem()
{
}

void LookAtSystem::initialize()
{

}

void LookAtSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();

	for(unsigned int i=0; i<p_entities.size(); i++ )
	{
		LookAtMainCamera_TAG* lookAtMainCamera = static_cast<LookAtMainCamera_TAG*>(
			p_entities[i]->getComponent( ComponentType::TAG_LookAtMainCamera) );

		auto stateSystem = static_cast<ServerStateSystem*>(
			m_world->getSystem(SystemType::ServerStateSystem));

		ServerStates state = ServerStates::NONE;
		if (stateSystem) state=stateSystem->getCurrentState();

		if (lookAtMainCamera && state==ServerStates::INGAME)
		{
			adaptDistanceBasedOnModules(p_entities[i]);
		}


		Transform* transform = static_cast<Transform*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		// optional component for lookat
		LookAtEntity* lookAt= static_cast<LookAtEntity*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::LookAtEntity) );

		// optional lookAt tags
		LookAtFollowMode_TAG* lookAtFollow=NULL;
		LookAtOrbitMode_TAG* lookAtOrbit=NULL;

		Component* t = p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::TAG_LookAtFollowMode );
		if (t!=NULL)
			lookAtFollow = static_cast<LookAtFollowMode_TAG*>(t);


		t = p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::TAG_LookAtOrbitMode );
		if (t!=NULL)
			lookAtOrbit = static_cast<LookAtOrbitMode_TAG*>(t);


		// Retrieve initial info
		AglVector3 position;
		if (transform) position = transform->getTranslation();
		AglQuaternion rotation = transform->getRotation();
		
		// Extract look-at entity and its transform
		Entity* targetEntity = m_world->getEntity(lookAt->getEntityId());
		Entity* offsetEntity = NULL;
		if (lookAt->m_offsetEntityId>=0) offsetEntity=m_world->getEntity(lookAt->m_offsetEntityId);
		if (targetEntity)
		{
			Transform* targetTransform = static_cast<Transform*>(
				targetEntity->getComponent(ComponentType::ComponentTypeIdx::Transform));
			Transform* origTransform = targetTransform;

			MeshOffsetTransform* meshOffset = static_cast<MeshOffsetTransform*>(
				targetEntity->getComponent( ComponentType::ComponentTypeIdx::MeshOffsetTransform ) );

			if (meshOffset)
			{
				AglMatrix mat = meshOffset->offset;
				targetTransform = new Transform(mat.transpose() * targetTransform->getMatrix());
			}
			else
			{
				targetTransform = new Transform(targetTransform->getMatrix());
			}

			AglVector3 lookTargetPos;
			if (targetTransform)
			{
				lookTargetPos = targetTransform->getTranslation();
			}

			//static AglVector3 m_velocity = AglVector3(0, 0, 0);
			//float springConstant = 64;
			//float springDamping = 8;

			// Follow behaviour
			if (lookAtFollow)
			{		
				// Set up look-at vars for the view matrix
				// Create offset vector from look-at component in the space of the target
				AglVector3 offset = AglVector3::zero();
				if (offsetEntity==NULL)
				{
					offset = lookAt->getFollowPositionOffset();
					offset.transformNormal(targetTransform->getMatrix());
				}
				else
				{
					Transform* offTransform = static_cast<Transform*>(
						offsetEntity->getComponent(ComponentType::ComponentTypeIdx::Transform));
					if (offTransform)
					{
						offset = lookAt->getFollowPositionOffset();
						offset.transformNormal(targetTransform->getMatrix());
						offset += offTransform->getTranslation()-lookTargetPos;
					}
				}

				// update transform

				// lerp
				if (lookAt->getSmoothMode()>0 && lookAt->getMoveSpd()*dt<1.0f)
				{
					AglVector3 dir = (lookTargetPos+offset)-position;
					float len = abs(dir.length());
					if (len>0.0f)
						dir.normalize();
					if (lookAt->getSmoothMode()==1)
					{
						position = AglVector3::lerp(position,lookTargetPos+offset,
							saturate( lookAt->getMoveSpd()*dt));
					}
					else if (lookAt->getSmoothMode()==2)
					{
						position = AglVector3::lerp(position,lookTargetPos+offset,
							saturate( (lookAt->getMoveSpd()/clamp(len,0.001f,1.0f) )*dt));				
					}						
					else if (lookAt->getSmoothMode()==3)
					{
						position = AglVector3::lerp(position,lookTargetPos+offset,
							saturate( lookAt->getMoveSpd()*dt));
						position += dir*max(0.0f,(AglVector3::length(position-lookTargetPos)-AglVector3::length(offset)));
					}					
					else if (lookAt->getSmoothMode()==4)
					{
						position = AglVector3::lerp(position,lookTargetPos+offset,
							saturate( (lookAt->getMoveSpd()/clamp(len,0.001f,1.0f) )*dt));
						position += dir*max(0.0f,(AglVector3::length(position-lookTargetPos)-AglVector3::length(offset)));
					}				
					else if (lookAt->getSmoothMode()==5)
					{
						lookAt->m_dir=dir;

						position += dir*lookAt->getMoveSpd()*dt;					
					}
					else if (lookAt->getSmoothMode()==6)
					{
						AglVector3 localOffset = lookAt->m_planeOffset;
						// localOffset = AglVector3::minOf(localOffset,offset*2.0f);
						float xbound = 15.0f;
						float ybound = 20.0f;
						float zbound = 10.0f;
						if (localOffset.x<-xbound) localOffset.x=-xbound;
						if (localOffset.x>xbound) localOffset.x=xbound;
						if (localOffset.y<-ybound) localOffset.y=-ybound;
						if (localOffset.y>ybound) localOffset.y=ybound;
						if (localOffset.z<-zbound) localOffset.z=-zbound;
						if (localOffset.z>zbound) localOffset.z=zbound;
						localOffset.transformNormal(origTransform->getMatrix());
						position = lookTargetPos+offset+localOffset;		
						// lookAt->m_planeOffset=AglVector3::lerp(lookAt->m_planeOffset,AglVector3::zero(),saturate(2.0f*dt));
					}
					// position += dir*max(0.0f,(AglVector3::length(position-lookTargetPos)-AglVector3::length(offset)));
				}
				else
				{
					position = lookTargetPos+offset;

					/*AglVector3 idealPosition = lookTargetPos+offset;
					AglVector3 displacement = position - idealPosition;
					AglVector3 springAcceleration = (displacement*(-springConstant)) - (m_velocity*springDamping);

					m_velocity += springAcceleration * dt;
					position += m_velocity * dt;*/
				}

				// slerp
// 				if (true || lookAt->getSmoothMode()<6)
// 				{
					if (lookAt->isRotationSmoothed() && lookAt->getRotationSpeed()*dt<1.0f)
					{
						rotation = AglQuaternion::slerp(rotation,targetTransform->getRotation(),
							saturate(lookAt->getRotationSpeed()*dt),true);
						rotation.normalize();
					}
					else
					{
						rotation = targetTransform->getRotation();
						if (lookAt->getSmoothMode()==6)
						{
							rotation *= AglQuaternion::constructFromAngularVelocity(lookAt->m_angleOffset);
						}
					}
// 				}
// 				else
// 				{
// 					AglVector3 angleOffset = lookAt->m_angleOffset;
// 					angleOffset.transformNormal(origTransform->getMatrix());
// 					rotation = AglQuaternion::constructFromAngularVelocity(angleOffset);
// 				}


				// update			
				transform->setRotation( rotation );
				transform->setTranslation( position );

			}
			// orbit behaviour
			else if (lookAtOrbit)
			{
				AglQuaternion interRotation = lookAt->getOrbitOffset();
				AglVector3 move = lookAt->getOrbitMovement()*lookAt->getOrbitRotationSpeed();
				if (move.length()>0.0f)	
					interRotation *= AglQuaternion::constructFromAngularVelocity(move*dt);
				interRotation.normalize();
				lookAt->setOrbitOffset(interRotation);

				rotation = AglQuaternion::slerp(rotation,targetTransform->getRotation()*interRotation,
					saturate(lookAt->getRotationSpeed()*0.7f*dt),true);
				rotation.normalize();

				AglVector3 offset = AglVector3::backward()*lookAt->getOrbitDistance();
				rotation.transformVector(offset);
				position = AglVector3::lerp(position, lookTargetPos + offset,
					saturate(lookAt->getMoveSpd()*3.0f*dt));


				// update
				transform->setTranslation( position );
				transform->setRotation( rotation );
			}
			// just lookat behaviour
			else if (!lookAtOrbit && !lookAtFollow)
			{
				AglVector3 dir = position-lookTargetPos;
				AglVector3::normalize(dir);
				transform->setForwardDirection(dir);
			}
			delete targetTransform;
		}
	}
		
}

void LookAtSystem::adaptDistanceBasedOnModules(Entity* p_entity)
{
	LookAtEntity* lookAt= static_cast<LookAtEntity*>(
		p_entity->getComponent( ComponentType::ComponentTypeIdx::LookAtEntity) );

	ShipManagerSystem* sms = static_cast<ShipManagerSystem*>(m_world->getSystem(SystemType::ShipManagerSystem));

	AglBoundingSphere bs = sms->findEditSphere(m_world->getEntity(lookAt->getEntityId()));

	float dt = m_world->getDelta();

	float orbitdist = (1-dt) * lookAt->getOrbitDistance() + dt*bs.radius*3;
	lookAt->setOrbitDistance(orbitdist);

	AglVector3 followdir = lookAt->getFollowPositionOffset();
	float followdist = followdir.length();
	followdir.normalize();

	followdist = (1-dt) * followdist + dt*bs.radius*3;
	lookAt->setFollowPositionOffset(followdir*followdist);
}
