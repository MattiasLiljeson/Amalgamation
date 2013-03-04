#include "SlotMarkerSystem.h"
#include "ParticleSystemsComponent.h"
#include "Transform.h"
#include "GraphicsBackendSystem.h"
#include "..\..\AgileAssets\src\AglStandardParticle.h"

SlotMarkerSystem::SlotMarkerSystem() : 
			  EntitySystem( SystemType::SlotMarkerSystem, 1,
							ComponentType::SlotMarker)
{
	m_current = -1;
}


SlotMarkerSystem::~SlotMarkerSystem()
{
}

void SlotMarkerSystem::initialize()
{

	GraphicsBackendSystem* gfx = static_cast<GraphicsBackendSystem*>(m_world->getSystem(SystemType::GraphicsBackendSystem));

	m_shipMarkerSize = AglVector2(0.1f, 0.1f*gfx->getAspectRatio());

	//Left slot
	slots[0] = m_world->createEntity();
	SlotMarker* sm = new SlotMarker();
	sm->shipMarker = createShipMarkerEntity(AglVector3(-1.0f + m_shipMarkerSize.x*0.5f, 0, 0), "1x1_solid_white.png", m_shipMarkerSize*0.5f)->getIndex();
	slots[0]->addComponent(ComponentType::SlotMarker, sm);
	m_world->addEntity(slots[0]);

	//Right slot
	slots[1] = m_world->createEntity();
	sm = new SlotMarker();
	sm->shipMarker = createShipMarkerEntity(AglVector3(1.0f - m_shipMarkerSize.x*0.5f, 0, 0), "1x1_solid_white.png", m_shipMarkerSize*0.5f)->getIndex();
	slots[1]->addComponent(ComponentType::SlotMarker, sm);
	m_world->addEntity(slots[1]);

	//Top
	slots[2] = m_world->createEntity();
	sm = new SlotMarker();
	sm->shipMarker = createShipMarkerEntity(AglVector3(0, -1.0f + m_shipMarkerSize.y*0.5f, 0), "1x1_solid_white.png", m_shipMarkerSize*0.5f)->getIndex();
	slots[2]->addComponent(ComponentType::SlotMarker, sm);
	m_world->addEntity(slots[2]);

	//Bottom
	slots[3] = m_world->createEntity();
	sm = new SlotMarker();
	sm->shipMarker = createShipMarkerEntity(AglVector3(0, 1.0f - m_shipMarkerSize.y*0.5f, 0), "1x1_solid_white.png", m_shipMarkerSize*0.5f)->getIndex();
	slots[3]->addComponent(ComponentType::SlotMarker, sm);
	m_world->addEntity(slots[3]);
}

void SlotMarkerSystem::processEntities( const vector<Entity*>& p_entities )
{
}

Entity* SlotMarkerSystem::createShipMarkerEntity(AglVector3 p_position, string p_texture, AglVector2 p_size)
{
	Entity* effect = m_world->createEntity();

	ParticleSystemsComponent* particleEmitter = new ParticleSystemsComponent();

	effect->addComponent( ComponentType::Transform, new Transform());

	AglParticleSystem ps;
	ps.setSpawnPoint(p_position);
	ps.setSpawnType(AglParticleSystemHeader::ONCE);
	ps.setAlignmentType(AglParticleSystemHeader::SCREEN);
	ps.getHeaderPtr()->blendMode = AglParticleSystemHeader::AglBlendMode_ALPHA;
	ps.setSpawnSpace(AglParticleSystemHeader::AglSpace_SCREEN);
	ps.setParticleSpace(AglParticleSystemHeader::AglSpace_SCREEN);
	ps.setParticleSize(p_size);
	ps.setParticleAge(100000);
	ps.setMaxOpacity(0.5f);

	ParticleSystemInstruction particleInstructionFlares;
	particleInstructionFlares.textureFileName = p_texture.c_str();
	particleInstructionFlares.particleSystem = ps;
	particleEmitter->addParticleSystemInstruction(particleInstructionFlares);

	effect->addComponent( ComponentType::ParticleSystemsComponent, particleEmitter);
	m_world->addEntity(effect);
	return effect;
}
void SlotMarkerSystem::setSlot(int p_index)
{
	if (m_current >= 0)
	{
		SlotMarker* toUnset = static_cast<SlotMarker*>(slots[m_current]->getComponent(ComponentType::SlotMarker));
		unset(toUnset);
	}
	m_current = p_index;
	if (m_current >= 0)
	{
		SlotMarker* toSet = static_cast<SlotMarker*>(slots[m_current]->getComponent(ComponentType::SlotMarker));
		set(toSet);
	}
}
void SlotMarkerSystem::set(SlotMarker* p_marker)
{
	Entity* shipMarker = m_world->getEntity(p_marker->shipMarker);
	ParticleSystemsComponent* ps = static_cast<ParticleSystemsComponent*>(
								shipMarker->getComponent(ComponentType::ParticleSystemsComponent));
	ps->getParticleSystemPtr(0)->setParticleSize(m_shipMarkerSize);
	ps->getParticleSystemPtr(0)->setMaxOpacity(1.0f);
	vector<AglStandardParticle>* particles = ps->getParticleSystemPtr(0)->getParticlesPtr();
	for (unsigned int i = 0; i < particles->size(); i++)
		((*particles)[i]).size = m_shipMarkerSize;
}
void SlotMarkerSystem::unset(SlotMarker* p_marker)
{
	Entity* shipMarker = m_world->getEntity(p_marker->shipMarker);
	ParticleSystemsComponent* ps = static_cast<ParticleSystemsComponent*>(
		shipMarker->getComponent(ComponentType::ParticleSystemsComponent));
	ps->getParticleSystemPtr(0)->setParticleSize(m_shipMarkerSize*0.5f);
	ps->getParticleSystemPtr(0)->setMaxOpacity(0.5f);
	vector<AglStandardParticle>* particles = ps->getParticleSystemPtr(0)->getParticlesPtr();
	for (unsigned int i = 0; i < particles->size(); i++)
		((*particles)[i]).size = m_shipMarkerSize*0.5f;

}