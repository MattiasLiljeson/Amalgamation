#include "SlotMarkerSystem.h"
#include "ParticleSystemsComponent.h"
#include "Transform.h"
#include "GraphicsBackendSystem.h"
#include <AglStandardParticle.h>
#include "ClientStateSystem.h"
#include <ResourceManager.h>
#include <GraphicsWrapper.h>
#include "ParticleSystemAndTexture.h"

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

}

void SlotMarkerSystem::processEntities( const vector<Entity*>& p_entities )
{
	ClientStateSystem* state = static_cast<ClientStateSystem*>(m_world->getSystem(SystemType::ClientStateSystem));

	if (state->getStateDelta(GameStates::INGAME) == EnumGameDelta::ENTEREDTHISFRAME)
	{

		GraphicsBackendSystem* gfx = static_cast<GraphicsBackendSystem*>(m_world->getSystem(SystemType::GraphicsBackendSystem));

		m_screenSize = gfx->getWindowSize();

		float resRatio = 1280 / m_screenSize.x;

		m_shipMarkerSize = AglVector2(0.15f, 0.15f*gfx->getAspectRatio());
		m_moduleMarkerSize = m_shipMarkerSize * 0.5f;

		gfx->getGfxWrapper()->
			createTexture("button_back.png", TEXTUREPATH );
		gfx->getGfxWrapper()->
			createTexture("rocketlaunchericon_activated.png", TEXTUREPATH );
		gfx->getGfxWrapper()->
			createTexture("icon_inactive.png", TEXTUREPATH );

		for (unsigned int i = 0; i < 10; i++)
		{
			m_textures[i].first 
				= gfx->getGfxWrapper()->createTexture("placeholder.png", TEXTUREPATH );
			m_textures[i].second
				= "placeholder.png";
		}

		//Speed booster
		m_textures[EntityType::BoosterModule - EntityType::ShipModuleStart].first 
			= gfx->getGfxWrapper()->createTexture("booster_icon_activated.png", TEXTUREPATH );
		m_textures[EntityType::BoosterModule - EntityType::ShipModuleStart].second
			= "booster_icon_activated.png";

		//Megagun
		m_textures[EntityType::MinigunModule - EntityType::ShipModuleStart].first 
			= gfx->getGfxWrapper()->createTexture("megagun_icon_activated.png", TEXTUREPATH );
		m_textures[EntityType::MinigunModule - EntityType::ShipModuleStart].second
			= "megagun_icon_activated.png";

		//Mine Layer
		m_textures[EntityType::MineLayerModule - EntityType::ShipModuleStart].first 
			= gfx->getGfxWrapper()->createTexture("minelayer_icon_activated.png", TEXTUREPATH );
		m_textures[EntityType::MineLayerModule - EntityType::ShipModuleStart].second
			= "minelayer_icon_activated.png";

		//Rocket launcher
		m_textures[EntityType::RocketLauncherModule - EntityType::ShipModuleStart].first 
			= gfx->getGfxWrapper()->createTexture("rocketlauncher_icon_activated.png", TEXTUREPATH );
		m_textures[EntityType::RocketLauncherModule - EntityType::ShipModuleStart].second
			= "rocketlauncher_icon_activated.png";

		//Shield
		m_textures[EntityType::ShieldModule - EntityType::ShipModuleStart].first 
			= gfx->getGfxWrapper()->createTexture("shield_icon_activated.png", TEXTUREPATH );
		m_textures[EntityType::ShieldModule - EntityType::ShipModuleStart].second
			= "shield_icon_activated.png";

		//Tesla
		m_textures[EntityType::TeslaCoilModule - EntityType::ShipModuleStart].first 
			= gfx->getGfxWrapper()->createTexture("tesla_icon_activated.png", TEXTUREPATH );
		m_textures[EntityType::TeslaCoilModule - EntityType::ShipModuleStart].second
			= "tesla_icon_activated.png";

		/*gfx->getGfxWrapper()->
			createTexture("Icon_Shield.png", GUI_TEXTURE_PATH );
		gfx->getGfxWrapper()->
			createTexture("Icon_Tesla.png", GUI_TEXTURE_PATH );*/

		positions[1] = AglVector3(0, 1.0f - m_shipMarkerSize.y*0.5f - 0.1f * resRatio/*For offseting from the timer, hardcoded right now*/, 0);
		positions[0] = AglVector3(0, -1.0f + m_shipMarkerSize.y*0.5f, 0);
		positions[2] = AglVector3(-1.0f + m_shipMarkerSize.x*0.5f, 0, 0);
		positions[3] = AglVector3(1.0f - m_shipMarkerSize.x*0.5f, 0, 0);

		//Left slot
		slots[2] = m_world->createEntity();
		SlotMarker* sm = new SlotMarker();
		sm->shipMarker = createShipMarkerEntity(positions[2], "button_left_back.png", m_shipMarkerSize*0.5f)->getIndex();
		sm->dir = AglVector3(1, 0, 0);
		slots[2]->addComponent(ComponentType::SlotMarker, sm);
		m_world->addEntity(slots[2]);

		//Right slot
		slots[3] = m_world->createEntity();
		sm = new SlotMarker();
		sm->shipMarker = createShipMarkerEntity(positions[3], "button_right_back.png", m_shipMarkerSize*0.5f)->getIndex();
		sm->dir = AglVector3(-1, 0, 0);
		slots[3]->addComponent(ComponentType::SlotMarker, sm);
		m_world->addEntity(slots[3]);

		//Bottom
		slots[0] = m_world->createEntity();
		sm = new SlotMarker();
		sm->shipMarker = createShipMarkerEntity(positions[0], "button_bottom_back.png", m_shipMarkerSize*0.5f)->getIndex();
		sm->dir = AglVector3(0, 1, 0);
		slots[0]->addComponent(ComponentType::SlotMarker, sm);
		m_world->addEntity(slots[0]);

		//Top
		slots[1] = m_world->createEntity();
		sm = new SlotMarker();
		sm->shipMarker = createShipMarkerEntity(positions[1], "button_top_back.png", m_shipMarkerSize*0.5f)->getIndex();
		sm->dir = AglVector3(0, -1, 0);
		slots[1]->addComponent(ComponentType::SlotMarker, sm);
		m_world->addEntity(slots[1]);
	}
	else if (state->getCurrentState() == GameStates::INGAME) 
	{
		GraphicsBackendSystem* gfx = static_cast<GraphicsBackendSystem*>(m_world->getSystem(SystemType::GraphicsBackendSystem));

		AglVector2 newSize = gfx->getWindowSize();

		if (newSize.x != m_screenSize.x || newSize.y != m_screenSize.y)
		{
			m_screenSize = newSize;
			float resRatio = 1280.0f / m_screenSize.x;

			m_shipMarkerSize = AglVector2(0.15f, 0.15f*gfx->getAspectRatio()) * resRatio;
			m_moduleMarkerSize = m_shipMarkerSize * 0.5f;

			positions[1] = AglVector3(0, 1.0f - m_shipMarkerSize.y*0.5f - 0.1f * resRatio /*For offseting from the timer, hardcoded right now*/, 0);
			positions[0] = AglVector3(0, -1.0f + m_shipMarkerSize.y*0.5f, 0);
			positions[2] = AglVector3(-1.0f + m_shipMarkerSize.x*0.5f, 0, 0);
			positions[3] = AglVector3(1.0f - m_shipMarkerSize.x*0.5f, 0, 0);

			reinit(0);
			reinit(1);
			reinit(2);
			reinit(3);
		}
	}
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
Entity* SlotMarkerSystem::createModuleMarkerEntity(AglVector3 p_position, string p_texture, AglVector2 p_size)
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

	arrangeChildren(p_marker, true);
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

	arrangeChildren(p_marker, false);
}
void SlotMarkerSystem::addMarker(int p_slot, EntityType p_type)
{
	if (p_slot >= 0)
	{
		SlotMarker* addTo = static_cast<SlotMarker*>(slots[p_slot]->getComponent(ComponentType::SlotMarker));
		Entity* newE = createModuleMarkerEntity(AglVector3(0, 0, 0), "rocketlaunchericon_activated.png", m_moduleMarkerSize);
		addTo->m_collection.push_back(pair<EntityType, Entity*>(p_type, newE));
		arrangeChildren(addTo, p_slot == m_current);
	}
}
void SlotMarkerSystem::removeMarker(int p_slot, EntityType p_type)
{
	if (p_slot >= 0)
	{
		SlotMarker* removeFrom = static_cast<SlotMarker*>(slots[p_slot]->getComponent(ComponentType::SlotMarker));
		for (unsigned int i = 0; i < removeFrom->m_collection.size(); i++)
		{
			if (removeFrom->m_collection[i].first.type == p_type.type)
			{
				m_world->deleteEntity(removeFrom->m_collection[i].second);
				
				removeFrom->m_collection[i] = removeFrom->m_collection.back();
				removeFrom->m_collection.pop_back();
				break;
			}
		}
		arrangeChildren(removeFrom, p_slot == m_current);
	}
}
void SlotMarkerSystem::arrangeChildren(SlotMarker* p_marker, bool p_marked)
{
	GraphicsBackendSystem* gfx = static_cast<GraphicsBackendSystem*>(
									m_world->getSystem(SystemType::GraphicsBackendSystem));

	ResourceManager<Texture>* textureManager = gfx->getGfxWrapper()->getTextureManager();

	Entity* shipMarker = m_world->getEntity(p_marker->shipMarker);
	ParticleSystemsComponent* parentPS = static_cast<ParticleSystemsComponent*>(
		shipMarker->getComponent(ComponentType::ParticleSystemsComponent));

	AglVector3 sourcePos = parentPS->getParticleSystemPtr(0)->getParticles()[0].position;


	float pi = 3.141592653589f;
	float step = 1;
	if (p_marker->m_collection.size() > 1)
		step = pi / (p_marker->m_collection.size()-1);
	float curr = 0;

	AglVector3 dir = p_marker->dir;

	if (p_marker->m_collection.size() > 2)
		dir = AglVector3(-dir.y, dir.x, 0);
	else if (p_marker->m_collection.size() > 1)
	{
		dir = AglVector3(-dir.y, dir.x, 0);
		step = pi / 3;
		AglMatrix rot(cos(step), -sin(step), 0, 0, sin(step), cos(step), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		dir.transformNormal(rot);
	}
	AglVector2 offset = AglVector2(m_shipMarkerSize.x, m_shipMarkerSize.y);
	if (!p_marked)
		offset *= 0.5f;
	for (unsigned int i = 0; i < p_marker->m_collection.size(); i++)
	{
		AglVector3 targetPos = sourcePos + AglVector3(dir.x * offset.x, dir.y * offset.y, 0);

		Entity* moduleMarker = p_marker->m_collection[i].second;
		ParticleSystemsComponent* ps = static_cast<ParticleSystemsComponent*>(
			moduleMarker->getComponent(ComponentType::ParticleSystemsComponent));

		ParticleSystemInstruction* inst = ps->getParticleSystemInstruction(0);
		if (inst)
		{
			ps->getParticleSystemInstruction(0)->particleSystem.setSpawnPoint(targetPos);
			ps->getParticleSystemInstruction(0)->particleSystem.setParticleSize(m_moduleMarkerSize * (0.5f + p_marked*0.5f));
			ps->getParticleSystemInstruction(0)->particleSystem.setMaxOpacity(0.5f + 0.5f*p_marked);
			if (p_marked)
			{
				string name = m_textures[p_marker->m_collection[i].first.type - EntityType::ShipModuleStart].second;
				ps->getParticleSystemInstruction(0)->textureFileName = name;
			}
			else
				ps->getParticleSystemInstruction(0)->textureFileName = "icon_inactive.png";
		}
		else
		{
			ps->getParticleSystemPtr(0)->setSpawnPoint(targetPos);
			ps->getParticleSystemPtr(0)->setMaxOpacity(0.5f + 0.5f*p_marked);

			if (p_marked)
			{
				string name = m_textures[p_marker->m_collection[i].first.type - EntityType::ShipModuleStart].second;
				ps->getParticleSystemAndTexturePtr(0)->textureIdx = textureManager->getResourceId(name);
			}
			else
			{
				ps->getParticleSystemAndTexturePtr(0)->textureIdx = textureManager->getResourceId("icon_inactive.png");
			}

			if (ps->getParticleSystemPtr(0)->getParticles().size() > 0)
			{
				vector<AglStandardParticle>* particles = ps->getParticleSystemPtr(0)->getParticlesPtr();
				for (unsigned int i = 0; i < particles->size(); i++)
				{
					((*particles)[i]).position = targetPos;
					((*particles)[i]).size = m_moduleMarkerSize * (0.5f + p_marked*0.5f);
				}
			}
		}

		AglMatrix rot(cos(step), -sin(step), 0, 0, sin(step), cos(step), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		dir.transformNormal(rot);
	}
}

void SlotMarkerSystem::reinit(int p_slot)
{
	SlotMarker* marker = static_cast<SlotMarker*>(slots[p_slot]->getComponent(ComponentType::SlotMarker));
	Entity* shipMarker = m_world->getEntity(marker->shipMarker);
	ParticleSystemsComponent* ps = static_cast<ParticleSystemsComponent*>(
		shipMarker->getComponent(ComponentType::ParticleSystemsComponent));
	ps->getParticleSystemPtr(0)->setParticleSize(m_shipMarkerSize * (0.5f + 0.5f*(p_slot == m_current)));
	ps->getParticleSystemPtr(0)->setMaxOpacity(0.5f+0.5f*(p_slot == m_current));
	ps->getParticleSystemPtr(0)->setSpawnPoint(positions[p_slot]);
	vector<AglStandardParticle>* particles = ps->getParticleSystemPtr(0)->getParticlesPtr();
	for (unsigned int i = 0; i < particles->size(); i++)
	{
		((*particles)[i]).size = m_shipMarkerSize * (0.5f + 0.5f*(p_slot == m_current));
		((*particles)[i]).position = positions[p_slot];
	}

	arrangeChildren(marker, p_slot == m_current);
}