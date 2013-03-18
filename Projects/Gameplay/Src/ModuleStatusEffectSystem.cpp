#include "ModuleStatusEffectSystem.h"
#include "ModuleStatusVisualizationMode.h"
#include "ClientStateSystem.h"
#include <ValueClamp.h>
#include "ColorTone.h"
#include "DestroyOnParticlesDeath.h"
#include <DebugUtil.h>
#include "NetsyncDirectMapperSystem.h"

ModuleStatusEffectSystem::ModuleStatusEffectSystem() : 
	EntitySystem( SystemType::ModuleStatusEffectSystem)
{
	m_myship=NULL;
	m_moduleColorTone = AglVector3(0.9f,0.8f,0.5f);
}

ModuleStatusEffectSystem::~ModuleStatusEffectSystem()
{

}

void ModuleStatusEffectSystem::clear()
{
	m_myship = NULL;

	m_unusedModuleEffects.clear();
	m_valueEffect.clear();
	m_healthEffects.clear();
	m_freefloatEffects.clear();
}

void ModuleStatusEffectSystem::initialize()
{

}

void ModuleStatusEffectSystem::process()
{
	// unused module effect
	while(!m_unusedModuleEffects.empty())
	{
		ModuleUnusedEffect data = m_unusedModuleEffects.back();
		// DEBUGWARNING((toString(data.score).c_str()));
		// createNumberEffectEntity(data);

		ParticleSystemsComponent* ps = static_cast<ParticleSystemsComponent*>(
										data.moduleEntity->getComponent(ComponentType::ParticleSystemsComponent));

		ModuleStatusVisualizationMode* visMode = static_cast<ModuleStatusVisualizationMode*>(
			data.moduleEntity->getComponent(ComponentType::ModuleStatusVisualizationMode));


		if (data.mode)
		{
			// create visualization mode component, to keep track
			if (!visMode)
			{
				visMode=new ModuleStatusVisualizationMode();
				data.moduleEntity->addComponent( ComponentType::ModuleStatusVisualizationMode, visMode );
				data.moduleEntity->applyComponentChanges();		
				// add particle effects for vismode
				// if there are existing particle effect components, they'll be appended
				// vismode keeps track of the new ones
			}
			if (!visMode->hasPositionHintParticle() && !visMode->hasUnusedHintParticles())
				ps = addAndRegisterUnusedModuleParticleEffect(data.moduleEntity, visMode);
			
			// update or activate effect based on new data
			// and the registered data in vismode
			if (ps)
			{
				activateUpdateUnusedModuleParticleEffect(data.moduleEntity, ps, visMode);
			}
		}
		else
		{
			// on removal an action is only needed if the component is already present
			// and has existing particles
			if (visMode && visMode->hasUnusedHintParticles() && 
				visMode->hasPositionHintParticle())
			{
				disableUnusedModuleParticleEffect(data.moduleEntity,ps,visMode);
			}
		}


		m_unusedModuleEffects.pop_back();
	}
	// module freefloat effect
	while (!m_freefloatEffects.empty())
	{
		ModuleFreefloatEffect data = m_freefloatEffects.back();
		updateFreefloatGlowEffect(data.moduleEntity,data.mode);
		m_freefloatEffects.pop_back();
	}
	//  module value effect
	while(!m_valueEffect.empty())
	{
		ModuleValueStatEffect data = m_valueEffect.back();
		m_valueEffect.pop_back();
	}
	// module spawn effect
	NetsyncDirectMapperSystem* directMapper = NULL;
	if (!m_spawnEffects.empty())
	{
		directMapper = static_cast<NetsyncDirectMapperSystem*>(m_world->getSystem(
			SystemType::NetsyncDirectMapperSystem));
		//
		while (!m_spawnEffects.empty())
		{
			ModuleSpawnEffect data = m_spawnEffects.back();
			Entity* moduleEntity = directMapper->getEntity(data.networkId);
			if (moduleEntity && createSpawnEffect(moduleEntity))
				m_spawnEffects.pop_back();
			else
				break;
		}
	}
	// module health effect
	while(!m_healthEffects.empty())
	{
		ModuleHealthStatEffect data = m_healthEffects.back();
		auto ps = static_cast<ParticleSystemsComponent*>(
			data.moduleEntity->getComponent(ComponentType::ParticleSystemsComponent));
		auto visMode = static_cast<ModuleStatusVisualizationMode*>(
			data.moduleEntity->getComponent(ComponentType::ModuleStatusVisualizationMode));

		if (data.health<=0.8f)
		{
			if (!visMode)
			{
				visMode=new ModuleStatusVisualizationMode();
				data.moduleEntity->addComponent( ComponentType::ModuleStatusVisualizationMode, visMode );
				data.moduleEntity->applyComponentChanges();		
				// add particle effects for vismode
			}
			if (!visMode->hasHealthParticles())
				ps = addAndRegisterDamagedModuleParticleEffect(data.moduleEntity,visMode);
			if (ps)
			{
				activateUpdateDamagedModuleParticleEffect(data.moduleEntity,ps,visMode,data.health);
			}
		}
		else
		{
			if (visMode && visMode->hasHealthParticles())
				disableDamagedParticleEffect(data.moduleEntity,ps,visMode);
		}

		m_healthEffects.pop_back();
	}
}

void ModuleStatusEffectSystem::setUnusedModuleEffect( ModuleUnusedEffect& p_fx )
{
	m_unusedModuleEffects.push_back(p_fx);
}

void ModuleStatusEffectSystem::setValueEffect( ModuleValueStatEffect& p_fx )
{
	m_valueEffect.push_back(p_fx);
}

void ModuleStatusEffectSystem::setHealthEffect( ModuleHealthStatEffect& p_fx )
{
	m_healthEffects.push_back(p_fx);
}


void ModuleStatusEffectSystem::setSpawnedEffect( ModuleSpawnEffect& p_fx )
{
	m_spawnEffects.push_back(p_fx);
}


void ModuleStatusEffectSystem::activateUpdateUnusedModuleParticleEffect(Entity* p_entity,
																		ParticleSystemsComponent* p_ps,
															ModuleStatusVisualizationMode* p_visMode)
{
	// Do updating here,
	// like distance based fading etc
	auto state = static_cast<ClientStateSystem*>(m_world->getSystem(SystemType::ClientStateSystem));
	
	//if (state->getStateDelta(GameStates::INGAME) == EnumGameDelta::ENTEREDTHISFRAME)
	if (!m_myship)
	{
		 m_myship = m_world->getEntityManager()->getFirstEntityByComponentType(ComponentType::TAG_MyShip);
	}
	if (m_myship)
	{
		if (p_ps)
		{
			float distSq = getDistSqBetweenShipAndModule(p_entity);
			// update positionhint
			AglParticleSystem* posHint = p_ps->getParticleSystemPtr(p_visMode->positionHintParticleSysId);
			if (posHint)
			{
				float tint = saturate(distSq/1000000.0f);
				posHint->setColor(AglVector4(tint,tint,tint,1.0f));
				if (tint<0.01f)
					posHint->setSpawnType(AglParticleSystemHeader::ONCE);
			}
			for (unsigned int i=0;i<p_visMode->unusedHintParticleSysId.size();i++)
			{
				AglParticleSystem* unusedHint = p_ps->getParticleSystemPtr(p_visMode->unusedHintParticleSysId[i]);
				if (unusedHint)
				{				
					float tint = saturate(distSq/50000.0f);
					unusedHint->setColor(AglVector4(tint,tint,tint,1.0f));
					if (tint<0.01f)
						unusedHint->setSpawnType(AglParticleSystemHeader::ONCE);
					if (distSq>1000000.0f)
						unusedHint->setSpawnType(AglParticleSystemHeader::ONCE);
					else if (unusedHint->getHeaderPtr()->spawnType == AglParticleSystemHeader::ONCE)
					{
						unusedHint->setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
						unusedHint->restart();
					}
				}
			}
		}
	}
}

ParticleSystemsComponent* ModuleStatusEffectSystem::addAndRegisterUnusedModuleParticleEffect( Entity* p_entity, ModuleStatusVisualizationMode* p_visMode )
{
	// Pure beauty below.
	// Enjoy!
	bool createNewParticleSystem=false;
	auto particleEmitters = static_cast<ParticleSystemsComponent*>(
		p_entity->getComponent(ComponentType::ParticleSystemsComponent));

	if (particleEmitters==NULL)
		createNewParticleSystem=true;

	if (createNewParticleSystem)
		particleEmitters = new ParticleSystemsComponent();

	float globalscale=3.0f;
	// World highlight, always on top
	// =============================================
	AglParticleSystem particleSystem1;
	AglVector3 offset = AglVector3( 0.0f,0.0f,0.0f);
	particleSystem1.setParticleSize(AglVector2(30.0f,30.0f)*globalscale);
	//
	particleSystem1.setSpawnPoint(offset);
	particleSystem1.setSpawnDirection(AglVector3::up());
	particleSystem1.setSpawnFrequency(0.5f);
	particleSystem1.setAlignmentType(AglParticleSystemHeader::SCREEN);
	particleSystem1.setParticlesPerSpawn(1);
	//
	particleSystem1.setSpawnSpace(AglParticleSystemHeader::AglSpace_LOCAL);
	particleSystem1.setParticleSpace( AglParticleSystemHeader::AglSpace_GLOBAL );
	particleSystem1.setParticleAge(2.0f);
	particleSystem1.setSpawnSpeed(0.0f);
	particleSystem1.setSpawnAngularVelocity(0.0f);
	particleSystem1.setFadeInStop(0.8f);
	particleSystem1.setFadeOutStart(1.10f);
	particleSystem1.setSpawnOffset(0.0f);
	particleSystem1.setSpread(0.0f);
	particleSystem1.setSpreadType(AglParticleSystemHeader::INSPACE);
	particleSystem1.setOffsetType(AglParticleSystemHeader::INSPHERE);
	particleSystem1.setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
	particleSystem1.getHeaderPtr()->blendMode=AglParticleSystemHeader::AglBlendMode_ADDITIVE;
	particleSystem1.getHeaderPtr()->rasterizerMode=AglParticleSystemHeader::AglRasterizerMode_ALWAYS_ON_TOP;
	// Create an instruction for creation
	ParticleSystemInstruction particleInstruction1;
	particleInstruction1.textureFileName = "module_spawned_marker.png";
	particleInstruction1.particleSystem = particleSystem1;
	// add instruction
	p_visMode->positionHintParticleSysId = particleEmitters->addParticleSystemInstruction(particleInstruction1);

	

	// art deco 1
	// =============================================
	AglParticleSystem particleSystem2;
	particleSystem2.setParticleSize(AglVector2(2.0f,2.0f)*globalscale);
	//
	particleSystem2.setSpawnPoint(offset);
	particleSystem2.setSpawnDirection(AglVector3::up());
	particleSystem2.setSpawnFrequency(0.3f);
	particleSystem2.setAlignmentType(AglParticleSystemHeader::SCREEN);
	particleSystem2.setParticlesPerSpawn(3);
	//			  
	particleSystem2.setSpawnSpace(AglParticleSystemHeader::AglSpace_LOCAL);
	particleSystem2.setParticleSpace( AglParticleSystemHeader::AglSpace_GLOBAL );
	particleSystem2.setParticleAge(4.0f);
	particleSystem2.setSpawnSpeed(2.0f);
	particleSystem2.setSpawnAngularVelocity(0.0f);
	particleSystem2.setFadeInStop(1.0f);
	particleSystem2.setFadeOutStart(3.00f);
	particleSystem2.setSpawnOffset(3.0f*globalscale);
	particleSystem2.setSpread(1.0f);
	particleSystem2.setSpreadType(AglParticleSystemHeader::INSPACE);
	particleSystem2.setOffsetType(AglParticleSystemHeader::ONSPHERE);
	particleSystem2.setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
	particleSystem2.getHeaderPtr()->blendMode=AglParticleSystemHeader::AglBlendMode_ADDITIVE;
	particleSystem2.getHeaderPtr()->rasterizerMode=AglParticleSystemHeader::AglRasterizerMode_Z_CULLED;
	// Create an instruction for creation
	ParticleSystemInstruction particleInstruction2;
	particleInstruction2.textureFileName = "decoparticle1.png";
	particleInstruction2.particleSystem = particleSystem2;
	// add instruction
	p_visMode->unusedHintParticleSysId.push_back(particleEmitters->addParticleSystemInstruction(particleInstruction2));
	// art deco 2
	// =============================================
	AglParticleSystem particleSystem3;
	particleSystem3.setParticleSize(AglVector2(1.0f,1.0f)*globalscale);
	//			  3
	particleSystem3.setSpawnPoint(offset);
	particleSystem3.setSpawnDirection(AglVector3::up());
	particleSystem3.setSpawnFrequency(1.0f);
	particleSystem3.setAlignmentType(AglParticleSystemHeader::SCREEN);
	particleSystem3.setParticlesPerSpawn(5);
	//			  3
	particleSystem3.setSpawnSpace(AglParticleSystemHeader::AglSpace_LOCAL);
	particleSystem3.setParticleSpace( AglParticleSystemHeader::AglSpace_GLOBAL );
	particleSystem3.setParticleAge(4.0f);
	particleSystem3.setSpawnSpeed(2.0f);
	particleSystem3.setSpawnAngularVelocity(2.0f);
	particleSystem3.setFadeInStop(2.0f);
	particleSystem3.setFadeOutStart(3.00f);
	particleSystem3.setSpawnOffset(2.0f*globalscale);
	particleSystem3.setSpread(1.0f);
	particleSystem3.setSpreadType(AglParticleSystemHeader::INSPACE);
	particleSystem3.setOffsetType(AglParticleSystemHeader::ONSPHERE);
	particleSystem3.setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
	particleSystem3.getHeaderPtr()->blendMode=AglParticleSystemHeader::AglBlendMode_ADDITIVE;
	particleSystem3.getHeaderPtr()->rasterizerMode=AglParticleSystemHeader::AglRasterizerMode_Z_CULLED;
	// Create an instruction for creation
	ParticleSystemInstruction particleInstruction3;
	particleInstruction3.textureFileName = "decoparticle2.png";
	particleInstruction3.particleSystem = particleSystem3;
	// add instruction
	p_visMode->unusedHintParticleSysId.push_back(particleEmitters->addParticleSystemInstruction(particleInstruction3));
	// art deco 3
	// =============================================
	AglParticleSystem particleSystem4;
	particleSystem4.setParticleSize(AglVector2(8.0f,8.0f)*globalscale);
	//			  4
	particleSystem4.setSpawnPoint(offset);
	particleSystem4.setSpawnDirection(AglVector3::up());
	particleSystem4.setSpawnFrequency(0.2f);
	particleSystem4.setAlignmentType(AglParticleSystemHeader::SCREEN);
	particleSystem4.setParticlesPerSpawn(1);
	//			  4
	particleSystem4.setSpawnSpace(AglParticleSystemHeader::AglSpace_LOCAL);
	particleSystem4.setParticleSpace( AglParticleSystemHeader::AglSpace_GLOBAL );
	particleSystem4.setParticleAge(10.0f);
	particleSystem4.setSpawnSpeed(0.10f);
	particleSystem4.setSpawnAngularVelocity(0.0f);
	particleSystem4.setFadeInStop(5.0f);
	particleSystem4.setFadeOutStart(5.00f);
	particleSystem4.setSpawnOffset(5.0f*globalscale);
	particleSystem4.setSpread(1.0f);
	particleSystem4.setSpreadType(AglParticleSystemHeader::INSPACE);
	particleSystem4.setOffsetType(AglParticleSystemHeader::ONSPHERE);
	particleSystem4.setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
	particleSystem4.getHeaderPtr()->blendMode=AglParticleSystemHeader::AglBlendMode_ADDITIVE;
	particleSystem4.getHeaderPtr()->rasterizerMode=AglParticleSystemHeader::AglRasterizerMode_Z_CULLED;
	// Create an instruction for creation
	ParticleSystemInstruction particleInstruction4;
	particleInstruction4.textureFileName = "decoparticle6.png";
	particleInstruction4.particleSystem = particleSystem4;
	// add instruction
	p_visMode->unusedHintParticleSysId.push_back(particleEmitters->addParticleSystemInstruction(particleInstruction4));
	// art deco 4
	// =============================================
	AglParticleSystem particleSystem5;
	particleSystem5.setParticleSize(AglVector2(2.0f,2.0f)*globalscale);
	//			  5
	particleSystem5.setSpawnPoint(offset);
	particleSystem5.setSpawnDirection(AglVector3::up());
	particleSystem5.setSpawnFrequency(1.0f);
	particleSystem5.setAlignmentType(AglParticleSystemHeader::SCREEN);
	particleSystem5.setParticlesPerSpawn(1);
	//			  5
	particleSystem5.setSpawnSpace(AglParticleSystemHeader::AglSpace_LOCAL);
	particleSystem5.setParticleSpace( AglParticleSystemHeader::AglSpace_GLOBAL );
	particleSystem5.setParticleAge(10.0f);
	particleSystem5.setSpawnSpeed(0.10f);
	particleSystem5.setSpawnAngularVelocity(0.0f);
	particleSystem5.setFadeInStop(1.0f);
	particleSystem5.setFadeOutStart(2.00f);
	particleSystem5.setSpawnOffset(10.0f*globalscale);
	particleSystem5.setSpread(0.0f);
	particleSystem5.setSpreadType(AglParticleSystemHeader::INSPACE);
	particleSystem5.setOffsetType(AglParticleSystemHeader::INSPHERE);
	particleSystem5.setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
	particleSystem5.getHeaderPtr()->blendMode=AglParticleSystemHeader::AglBlendMode_ADDITIVE;
	particleSystem5.getHeaderPtr()->rasterizerMode=AglParticleSystemHeader::AglRasterizerMode_Z_CULLED;
	// Create an instruction for creation
	ParticleSystemInstruction particleInstruction5;
	particleInstruction5.textureFileName = "decoparticle3.png";
	particleInstruction5.particleSystem = particleSystem5;
	// add instruction
	p_visMode->unusedHintParticleSysId.push_back(particleEmitters->addParticleSystemInstruction(particleInstruction5));
	// art deco 5
	// =============================================
	AglParticleSystem particleSystem6;
	particleSystem6.setParticleSize(AglVector2(2.0f,2.0f)*globalscale);
	//			  6
	particleSystem6.setSpawnPoint(offset);
	particleSystem6.setSpawnDirection(AglVector3::up());
	particleSystem6.setSpawnFrequency(1.0f);
	particleSystem6.setAlignmentType(AglParticleSystemHeader::SCREEN);
	particleSystem6.setParticlesPerSpawn(1);
	//			  6
	particleSystem6.setSpawnSpace(AglParticleSystemHeader::AglSpace_LOCAL);
	particleSystem6.setParticleSpace( AglParticleSystemHeader::AglSpace_GLOBAL );
	particleSystem6.setParticleAge(10.0f);
	particleSystem6.setSpawnSpeed(0.30f);
	particleSystem6.setSpawnAngularVelocity(1.0f);
	particleSystem6.setFadeInStop(2.0f);
	particleSystem6.setFadeOutStart(2.00f);
	particleSystem6.setSpawnOffset(5.0f*globalscale);
	particleSystem6.setSpread(0.3f);
	particleSystem6.setSpreadType(AglParticleSystemHeader::INSPACE);
	particleSystem6.setOffsetType(AglParticleSystemHeader::ONSPHERE);
	particleSystem6.setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
	particleSystem6.getHeaderPtr()->blendMode=AglParticleSystemHeader::AglBlendMode_ADDITIVE;
	particleSystem6.getHeaderPtr()->rasterizerMode=AglParticleSystemHeader::AglRasterizerMode_Z_CULLED;
	// Create an instruction for creation
	ParticleSystemInstruction particleInstruction6;
	particleInstruction6.textureFileName = "decoparticle5.png";
	particleInstruction6.particleSystem = particleSystem6;
	// add instruction
	p_visMode->unusedHintParticleSysId.push_back(particleEmitters->addParticleSystemInstruction(particleInstruction6));
	// art deco 7
	// =============================================
	AglParticleSystem particleSystem7;
	particleSystem7.setParticleSize(AglVector2(14.0f,14.0f)*globalscale);
	//			  7
	particleSystem7.setSpawnPoint(offset);
	particleSystem7.setSpawnDirection(AglVector3::up());
	particleSystem7.setSpawnFrequency(0.15f);
	particleSystem7.setAlignmentType(AglParticleSystemHeader::SCREEN);
	particleSystem7.setParticlesPerSpawn(1);
	//			  7
	particleSystem7.setSpawnSpace(AglParticleSystemHeader::AglSpace_LOCAL);
	particleSystem7.setParticleSpace( AglParticleSystemHeader::AglSpace_GLOBAL );
	particleSystem7.setParticleAge(10.0f);
	particleSystem7.setSpawnSpeed(0.10f);
	particleSystem7.setSpawnAngularVelocity(0.0f);
	particleSystem7.setFadeInStop(5.0f);
	particleSystem7.setFadeOutStart(5.00f);
	particleSystem7.setSpawnOffset(1.0f*globalscale);
	particleSystem7.setSpread(0.3f);
	particleSystem7.setSpreadType(AglParticleSystemHeader::INSPACE);
	particleSystem7.setOffsetType(AglParticleSystemHeader::ONSPHERE);
	particleSystem7.setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
	particleSystem7.getHeaderPtr()->blendMode=AglParticleSystemHeader::AglBlendMode_ADDITIVE;
	particleSystem7.getHeaderPtr()->rasterizerMode=AglParticleSystemHeader::AglRasterizerMode_Z_CULLED;
	// Create an instruction for creation
	ParticleSystemInstruction particleInstruction7;
	particleInstruction7.textureFileName = "decoparticle4.png";
	particleInstruction7.particleSystem = particleSystem7;
	// add instruction
	p_visMode->unusedHintParticleSysId.push_back(particleEmitters->addParticleSystemInstruction(particleInstruction7));

	// add it
	if (createNewParticleSystem)
		p_entity->addComponent( ComponentType::ParticleSystemsComponent, particleEmitters );


	p_entity->applyComponentChanges();

	return particleEmitters;
}

void ModuleStatusEffectSystem::disableUnusedModuleParticleEffect( Entity* p_entity, 
													 ParticleSystemsComponent* p_ps,
													 ModuleStatusVisualizationMode* p_visMode)
{
	if (p_ps!=NULL)
	{
		// let the particles have one last run and "die" silently by setting spawn type to once
		AglParticleSystem* posHint = p_ps->getParticleSystemPtr(p_visMode->positionHintParticleSysId);
		if (posHint) 
		{
			posHint->setSpawnType(AglParticleSystemHeader::ONCE);
			//posHint->setColor(AglVector4(0, 0, 0, 0));
		}
		for (unsigned int i=0;i<p_visMode->unusedHintParticleSysId.size();i++)
		{
			AglParticleSystem* unusedHint = p_ps->getParticleSystemPtr(p_visMode->unusedHintParticleSysId[i]);
			if (unusedHint) 
			{
				unusedHint->setSpawnType(AglParticleSystemHeader::ONCE);
				//unusedHint->setColor(AglVector4(0, 0, 0, 0));
			}
		}
	}
}

void ModuleStatusEffectSystem::updateFreefloatGlowEffect( Entity* p_entity, bool p_mode )
{
	float distSq = getDistSqBetweenShipAndModule(p_entity);
	float glow = 0.0f;
	if (p_mode) glow = 1.0f-saturate(distSq/1000000.0f-100.0f);
	auto glowEffect = static_cast<ColorTone*>(
		p_entity->getComponent(ComponentType::ColorTone));
	if (glow>0.001f)
	{
		if (!glowEffect)
		{
			glowEffect = new ColorTone();
			p_entity->addComponent( ComponentType::ColorTone, glowEffect );
			p_entity->applyComponentChanges();
		}
		glowEffect->color=AglVector4(m_moduleColorTone.x,
			m_moduleColorTone.y,m_moduleColorTone.z,1.0f)*glow;
		glowEffect->toneEnabled=true;
	}
	else
	{
		if (glowEffect)
			glowEffect->toneEnabled=false;
	}
	//

}

float ModuleStatusEffectSystem::getDistSqBetweenShipAndModule(Entity* p_moduleEntity)
{
	float distSq = 0.0f;
	if (m_myship)
	{
		auto shiptransform = static_cast<Transform*>(
			m_myship->getComponent(ComponentType::Transform));
		auto moduletransform = static_cast<Transform*>(
			p_moduleEntity->getComponent(ComponentType::Transform));
		if (shiptransform && moduletransform)
		{
			AglVector3 shippos = shiptransform->getTranslation();
			AglVector3 modulepos = moduletransform->getTranslation();
			distSq = AglVector3::lengthSquared(modulepos-shippos);
		}
	}
	return distSq;
}

void ModuleStatusEffectSystem::setFreefloatEffect( ModuleFreefloatEffect& p_fx )
{
	m_freefloatEffects.push_back(p_fx);
}

void ModuleStatusEffectSystem::activateUpdateDamagedModuleParticleEffect( Entity* p_entity, 
																		 ParticleSystemsComponent* p_ps, 
																		 ModuleStatusVisualizationMode* p_visMode, 
																		 float p_health )
{
	if (p_ps!=NULL)
	{
		// let the particles have one last run and "die" silently by setting spawn type to once
		AglParticleSystem* dmg = p_ps->getParticleSystemPtr(p_visMode->healthParticlesSysId[0]);
		if (dmg)
		{
			dmg->setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
			dmg->setParticlesPerSpawn(2+(int)((1.0f-p_health)*10.0f));
			dmg->setSpawnOffset(2+(int)(1.0f-p_health));
		}
		/*
		dmg = p_ps->getParticleSystemPtr(p_visMode->healthParticlesSysId[1]);
		if (dmg)
		{
			dmg->setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
		}*/
	}
}

ParticleSystemsComponent* ModuleStatusEffectSystem::addAndRegisterDamagedModuleParticleEffect( Entity* p_entity, 
																		  ModuleStatusVisualizationMode* p_visMode )
{
	bool createNewParticleSystem=false;
	auto particleEmitters = static_cast<ParticleSystemsComponent*>(
		p_entity->getComponent(ComponentType::ParticleSystemsComponent));

	if (particleEmitters==NULL)
		createNewParticleSystem=true;

	if (createNewParticleSystem)
		particleEmitters = new ParticleSystemsComponent();

	float globalscale=1.0f;
	// fire
	// =============================================
	AglParticleSystem particleSystem1;
	AglVector3 offset = AglVector3( 0.0f,0.0f,0.0f);
	particleSystem1.setParticleSize(AglVector2(5.0f,5.0f)*globalscale);
	particleSystem1.setColor(AglVector4(0.63f,0.2f,0.0353f,1.0f));
	particleSystem1.setMaxOpacity(0.7f);
	//
	particleSystem1.setSpawnPoint(offset);
	particleSystem1.setSpawnDirection(AglVector3::up());
	particleSystem1.setSpawnFrequency(10.0f);
	particleSystem1.setAlignmentType(AglParticleSystemHeader::SCREEN);
	particleSystem1.setParticlesPerSpawn(2);
	//
	particleSystem1.setSpawnSpace(AglParticleSystemHeader::AglSpace_LOCAL);
	particleSystem1.setParticleSpace( AglParticleSystemHeader::AglSpace_LOCAL );
	particleSystem1.setParticleAge(1.0f);
	particleSystem1.setSpawnSpeed(0.1f);
	particleSystem1.setSpawnAngularVelocity(0.4f);
	particleSystem1.setFadeInStop(0.5f);
	particleSystem1.setFadeOutStart(0.5f);
	particleSystem1.setSpawnOffset(2.0f);
	particleSystem1.setSpread(1.0f);
	particleSystem1.setSpreadType(AglParticleSystemHeader::INSPACE);
	particleSystem1.setOffsetType(AglParticleSystemHeader::INSPHERE);
	particleSystem1.setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
	particleSystem1.getHeaderPtr()->blendMode=AglParticleSystemHeader::AglBlendMode_ADDITIVE;
	particleSystem1.getHeaderPtr()->rasterizerMode=AglParticleSystemHeader::AglRasterizerMode_Z_CULLED;
	// Create an instruction for creation
	ParticleSystemInstruction particleInstruction1;
	particleInstruction1.textureFileName = "additive_flame.png";
	particleInstruction1.particleSystem = particleSystem1;
	// add instruction
	p_visMode->healthParticlesSysId.push_back(particleEmitters->addParticleSystemInstruction(particleInstruction1));
	// smoke
	// =============================================
	// World highlight, always on top
	// =============================================
	/*
	AglParticleSystem particleSystem2;
	particleSystem2.setParticleSize(AglVector2(8.0f,8.0f)*globalscale);
	particleSystem2.setColor(AglVector4(0.4f,0.4f,0.4f,1.0f));
	particleSystem2.setMaxOpacity(1.0f);
	//			  2
	particleSystem2.setSpawnPoint(offset);
	particleSystem2.setSpawnDirection(AglVector3::up());
	particleSystem2.setSpawnFrequency(10.0f);
	particleSystem2.setAlignmentType(AglParticleSystemHeader::SCREEN);
	particleSystem2.setParticlesPerSpawn(3);
	//			  2
	particleSystem2.setSpawnSpace(AglParticleSystemHeader::AglSpace_LOCAL);
	particleSystem2.setParticleSpace( AglParticleSystemHeader::AglSpace_GLOBAL );
	particleSystem2.setParticleAge(0.2f);
	particleSystem2.setSpawnSpeed(0.2f);
	particleSystem2.setSpawnAngularVelocity(0.2f);
	particleSystem2.setFadeInStop(0.05f);
	particleSystem2.setFadeOutStart(0.1f);
	particleSystem2.setSpawnOffset(0.0f);
	particleSystem2.setSpread(1.0f);
	particleSystem2.setSpreadType(AglParticleSystemHeader::INSPACE);
	particleSystem2.setOffsetType(AglParticleSystemHeader::ONSPHERE);
	particleSystem2.setSpawnType(AglParticleSystemHeader::CONTINUOUSLY);
	particleSystem2.getHeaderPtr()->blendMode=AglParticleSystemHeader::AglBlendMode_ALPHA;
	particleSystem2.getHeaderPtr()->rasterizerMode=AglParticleSystemHeader::AglRasterizerMode_Z_CULLED;
	// Create an instruction for creation
	ParticleSystemInstruction particleInstruction2;
	particleInstruction2.textureFileName = "smoke.png";
	particleInstruction2.particleSystem = particleSystem2;
	// add instruction
	p_visMode->healthParticlesSysId.push_back(particleEmitters->addParticleSystemInstruction(particleInstruction2));
	*/
	// add it
	if (createNewParticleSystem)
		p_entity->addComponent( ComponentType::ParticleSystemsComponent, particleEmitters );

	p_entity->applyComponentChanges();

	return particleEmitters;
}

void ModuleStatusEffectSystem::disableDamagedParticleEffect( Entity* p_entity, 
															ParticleSystemsComponent* p_ps,
															ModuleStatusVisualizationMode* p_visMode )
{
	if (p_ps!=NULL)
	{
		// let the particles have one last run and "die" silently by setting spawn type to once
		AglParticleSystem* dmg = p_ps->getParticleSystemPtr(p_visMode->healthParticlesSysId[0]);
		if (dmg)
		{
			dmg->setSpawnType(AglParticleSystemHeader::ONCE);
			dmg->setParticlesPerSpawn(2);
			dmg->setSpawnOffset(2.0f);
		}/*
		dmg = p_ps->getParticleSystemPtr(p_visMode->healthParticlesSysId[1]);
		if (dmg)
		{
			dmg->setSpawnType(AglParticleSystemHeader::ONCE);
		}*/
	}
}

bool ModuleStatusEffectSystem::createSpawnEffect( Entity* p_entity )
{
	Entity* effect = m_world->createEntity();

	auto othertransform = static_cast<Transform*>(
		p_entity->getComponent(ComponentType::Transform));

	if (othertransform)
	{
		Transform* transform = new Transform(othertransform->getTranslation(), 
			AglQuaternion::identity(),AglVector3::one());
		effect->addComponent( ComponentType::Transform, transform );


		ParticleSystemsComponent* particleEmitters = new ParticleSystemsComponent();

		effect->addComponent(ComponentType::DestroyOnParticlesDeath,
			new DestroyOnParticlesDeath());

		float size=4.0f;
		for (unsigned int i=0;i<(unsigned int)size;i++)
		{
			AglParticleSystem particleSystem;
			particleSystem.getHeaderPtr()->rasterizerMode = AglParticleSystemHeader::AglRasterizerMode_ALWAYS_ON_TOP;
			particleSystem.getHeaderPtr()->blendMode=AglParticleSystemHeader::AglBlendMode_ADDITIVE;
			AglVector3 offset = AglVector3::zero();
			particleSystem.setParticleSize(AglVector2(20.0f,20.0f)*5.0f*(1.0f+(float)i));
			// offset.transform(cameraRot);
			particleSystem.setSpawnPoint(offset);
			particleSystem.setSpawnDirection(AglVector3::up());
			particleSystem.setSpawnFrequency(10.0f-(float)i);
			particleSystem.setAlignmentType(AglParticleSystemHeader::SCREEN);
			// particleSystem.set
			particleSystem.setSpawnSpace(AglParticleSystemHeader::AglSpace_LOCAL);
			particleSystem.setParticleSpace( AglParticleSystemHeader::AglSpace_GLOBAL );
			particleSystem.setSpawnType(AglParticleSystemHeader::ONCE);
			particleSystem.setParticleAge(4.0f+i*2.0f);
			particleSystem.setSpawnSpeed(0.0f);
			particleSystem.setFadeInStop(1.0f+i*2.0f);
			particleSystem.setFadeOutStart(3.0f+i*2.0f);
			// Create an instruction for creation
			ParticleSystemInstruction particleInstruction;
			particleInstruction.textureFileName = "decoparticle6.png";
			particleInstruction.particleSystem = particleSystem;
			// add instruction
			particleEmitters->addParticleSystemInstruction(particleInstruction);
		}

		effect->addComponent( ComponentType::ParticleSystemsComponent, particleEmitters );
	
		m_world->addEntity(effect);
		return true;
	}

	return false;

	// return effect;
}
