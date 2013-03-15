#include "HudSystem.h"

#include "HudElement.h"
#include "LibRocketBackendSystem.h"
#include <Globals.h>
#include "ClientStateSystem.h"
#include "ParticleSystemsComponent.h"
#include <AglParticleSystem.h>
#include "Transform.h"
#include "GraphicsBackendSystem.h"
#include "GameplayTags.h"
#include <TcpClient.h>

HudSystem::HudSystem( LibRocketBackendSystem* p_backend, TcpClient* p_client )
	: EntitySystem( SystemType::HudSystem )
{
	m_backend			= p_backend;
	m_hudVisible		= false;
	m_currentState		= false;
	m_client			= p_client;
	m_hudIndex			= -1;
	m_leftWing			= NULL;
	m_rightWing			= NULL;
	m_timerMonitor		= NULL;
	m_constructionMode	= NULL;
}


HudSystem::~HudSystem()
{
}

void HudSystem::initialize()
{
	m_hudIndex = m_backend->loadDocument( GUI_HUD_PATH.c_str(), "hud" );

	// m_backend->showDocument(m_hudIndex);
}

void HudSystem::process()
{
	ClientStateSystem* stateSystem = static_cast<ClientStateSystem*>(m_world->
		getSystem(SystemType::ClientStateSystem));
	
	if(stateSystem->getStateDelta(GameStates::INGAME) == EnumGameDelta::ENTEREDTHISFRAME)
	{
		m_backend->showDocument(m_hudIndex);
		m_hudVisible = true;

		setHUDData(SCORE,"-1");
		setHUDData(TIME,"00:00");
		setHUDData(SERVERNAME, m_client->getServerName().c_str());

		GraphicsBackendSystem* gfx = static_cast<GraphicsBackendSystem*>(m_world->getSystem(SystemType::GraphicsBackendSystem));

		m_screenSize = gfx->getWindowSize();

		float screenRatio = gfx->getAspectRatio();

		/*float wingRatio = 228.0f / 527.0f;
		AglVector2 wingsize = AglVector2(1.0f, 1.0f*gfx->getAspectRatio() * wingRatio) * screenRatio;

		float timerRatio = 109.0f / 953.0f;
		AglVector2 timerSize = AglVector2(0.4f, 0.4f*gfx->getAspectRatio() * timerRatio) * screenRatio;
		*/
		float constRatio = 65.0f / 1025.0f;
		AglVector2 constSize = AglVector2(0.6f, 0.6f*gfx->getAspectRatio() * constRatio) * screenRatio;

		/*m_leftWing = createSprite(AglVector3(-1.0f+wingsize.x*0.5f, 1.0f-wingsize.y*0.5f, 0.0f), "leftwing_HUD.png", wingsize);
		m_rightWing = createSprite(AglVector3(1.0f-wingsize.x*0.5f, 1.0f-wingsize.y*0.5f, 0.0f), "rightwing_HUD.png", wingsize);
		m_timerMonitor = createSprite(AglVector3(0.0f, 1.0f-timerSize.y, 0.0f), "timer_HUD.png", timerSize);*/


		m_constructionMode = createConstructionSprite(
			AglVector3(0.0f, -1.0f-constSize.y + 0.5f, 0.0f), 
			"construction_mode_label.png", constSize);
	}
	else if (stateSystem->getCurrentState() == GameStates::INGAME)
	{
		GraphicsBackendSystem* gfx = static_cast<GraphicsBackendSystem*>
			(m_world->getSystem(SystemType::GraphicsBackendSystem));

		AglVector2 newSize = gfx->getWindowSize();

		if (newSize.x != m_screenSize.x || newSize.y != m_screenSize.y)
		{
			m_screenSize = newSize;
			float screenRatio = 1280 / m_screenSize.x;

			/*float wingRatio = 228.0f / 527.0f;
			AglVector2 wingsize = AglVector2(0.3f, 0.3f*gfx->getAspectRatio() * wingRatio) * screenRatio;

			float timerRatio = 109.0f / 953.0f;
			AglVector2 timerSize = AglVector2(0.4f, 0.4f*gfx->getAspectRatio() * timerRatio) * screenRatio;
			*/
			float constRatio = 65.0f / 1025.0f;
			AglVector2 constSize = AglVector2(0.6f, 0.6f*gfx->getAspectRatio() * constRatio) * screenRatio;
			/*
			reinitSprite(m_leftWing, AglVector3(-1.0f+wingsize.x*0.5f, 1.0f-wingsize.y*0.5f, 0.0f), wingsize);
			reinitSprite(m_rightWing, AglVector3(1.0f-wingsize.x*0.5f, 1.0f-wingsize.y*0.5f, 0.0f), wingsize);
			reinitSprite(m_timerMonitor, AglVector3(0.0f, 1.0f-timerSize.y, 0.0f), timerSize);*/
			
			reinitSprite(m_constructionMode, AglVector3(0.0f, -1.0f-constSize.y + 0.5f, 0.0f), constSize);
		}
		
		EntityManager* entitymanager = m_world->getEntityManager();
		
		Entity* ship = entitymanager->getFirstEntityByComponentType(ComponentType::TAG_MyShip);
		ShipEditMode_TAG* editMode = static_cast<ShipEditMode_TAG*>(ship->getComponent(ComponentType::TAG_ShipEditMode));
		if (editMode)
		{
			ParticleSystemsComponent* ps = static_cast<ParticleSystemsComponent*>(
				m_constructionMode->getComponent(ComponentType::ParticleSystemsComponent));
			ps->getParticleSystemPtr(0)->getHeaderPtr()->color = AglVector4(1, 1, 1, 1);
		}
		else
		{
			
			ParticleSystemsComponent* ps = static_cast<ParticleSystemsComponent*>(
				m_constructionMode->getComponent(ComponentType::ParticleSystemsComponent));
			AglParticleSystem* partSystem = ps->getParticleSystemPtr(0);
			if(partSystem){
				partSystem->getHeaderPtr()->color = AglVector4(0, 0, 0, 0);
			}
		}
		

	}
	else if(stateSystem->getStateDelta(GameStates::RESULTS) == EnumGameDelta::ENTEREDTHISFRAME){
		m_backend->hideDocument(m_hudIndex);
		m_hudVisible = false;
	}
}

void HudSystem::setHUDVisibility(bool p_setVisibility)
{
	if (m_hudVisible == p_setVisibility)
		return;

	m_hudVisible = p_setVisibility;

	if (m_hudVisible)
		m_backend->showDocument(m_hudIndex);
	else
		m_backend->hideDocument(m_hudIndex);
}

void HudSystem::setHUDData( HUD_TYPES p_type, const char* p_value )
{
	switch (p_type)
	{
	case HudSystem::TIME:
		m_backend->updateElement(m_hudIndex,TIMERELEMENT,p_value);
		break;
	case HudSystem::SCORE:
		m_backend->updateElement(m_hudIndex,SCOREELEMENT,p_value);
		break;
	case HudSystem::MAPPING:
		m_backend->updateElement(m_hudIndex,MAPPINGELEMENT,p_value);
		break;
	case HudSystem::PLAYERNAME:
		m_backend->updateElement(m_hudIndex,PLAYERELEMENT,p_value);
		break;
	case HudSystem::SERVERNAME:
		m_backend->updateElement(m_hudIndex, SERVERELEMENT,p_value);
		break;
	default:
		break;
	}
}

Entity* HudSystem::createSprite(AglVector3 p_position, string p_texture, AglVector2 p_size)
{
	Entity* sprite = m_world->createEntity();

	ParticleSystemsComponent* particleEmitter = new ParticleSystemsComponent();

	sprite->addComponent( ComponentType::Transform, new Transform());

	AglParticleSystem ps;
	ps.setSpawnPoint(p_position);
	ps.setSpawnType(AglParticleSystemHeader::ONCE);
	ps.setAlignmentType(AglParticleSystemHeader::SCREEN);
	ps.getHeaderPtr()->blendMode = AglParticleSystemHeader::AglBlendMode_ALPHA;
	ps.setSpawnSpace(AglParticleSystemHeader::AglSpace_SCREEN);
	ps.setParticleSpace(AglParticleSystemHeader::AglSpace_SCREEN);
	ps.setParticleSize(p_size);
	ps.setParticleAge(100000);
	ps.setMaxOpacity(1.0f);

	ParticleSystemInstruction particleInstructionFlares;
	particleInstructionFlares.textureFileName = p_texture.c_str();
	particleInstructionFlares.particleSystem = ps;
	particleEmitter->addParticleSystemInstruction(particleInstructionFlares);

	sprite->addComponent( ComponentType::ParticleSystemsComponent, particleEmitter);
	m_world->addEntity(sprite);
	return sprite;
}
Entity* HudSystem::createConstructionSprite(AglVector3 p_position, string p_texture, 
											AglVector2 p_size)
{
	Entity* sprite = m_world->createEntity();

	ParticleSystemsComponent* particleEmitter = new ParticleSystemsComponent();

	sprite->addComponent( ComponentType::Transform, new Transform());

	AglParticleSystem ps;
	ps.setSpawnPoint(p_position);
	ps.setSpawnType(AglParticleSystemHeader::ONCE);
	ps.setAlignmentType(AglParticleSystemHeader::SCREEN);
	ps.getHeaderPtr()->blendMode = AglParticleSystemHeader::AglBlendMode_ALPHA;
	ps.setSpawnSpace(AglParticleSystemHeader::AglSpace_SCREEN);
	ps.setParticleSpace(AglParticleSystemHeader::AglSpace_SCREEN);
	ps.setParticleSize(p_size);
	ps.setParticleAge(10000000.0f);
	ps.setMaxOpacity(1.0f);
	ps.setFadeOutStart(10000000.0f);
	ps.setFadeInStop(0.0f);
	ps.setSpawnFrequency(1.0f);

	ParticleSystemInstruction particleInstructionFlares;
	particleInstructionFlares.textureFileName = p_texture.c_str();
	particleInstructionFlares.particleSystem = ps;
	particleEmitter->addParticleSystemInstruction(particleInstructionFlares);

	sprite->addComponent( ComponentType::ParticleSystemsComponent, particleEmitter);
	m_world->addEntity(sprite);
	return sprite;
}
void HudSystem::reinitSprite(Entity* p_sprite, AglVector3 p_position, AglVector2 p_size)
{
	ParticleSystemsComponent* ps = static_cast<ParticleSystemsComponent*>(
		p_sprite->getComponent(ComponentType::ParticleSystemsComponent));
	ps->getParticleSystemPtr(0)->setParticleSize(p_size);
	ps->getParticleSystemPtr(0)->setSpawnPoint(p_position);
	vector<AglStandardParticle>* particles = ps->getParticleSystemPtr(0)->getParticlesPtr();
	for (unsigned int i = 0; i < particles->size(); i++)
	{
		((*particles)[i]).size = p_size;
		((*particles)[i]).position = p_position;
	}
}
