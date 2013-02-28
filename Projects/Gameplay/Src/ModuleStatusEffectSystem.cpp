#include "ModuleStatusEffectSystem.h"

ModuleStatusEffectSystem::ModuleStatusEffectSystem() : 
	EntitySystem( SystemType::ModuleStatusEffectSystem)
{

}

ModuleStatusEffectSystem::~ModuleStatusEffectSystem()
{

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
		if (!data.mode)
			addParticleEffectComponent(data.moduleEntity);
		else
			removeParticleEffectComponent(data.moduleEntity);
		m_unusedModuleEffects.pop_back();
	}
	//  module value effect
	while(!m_valueEffect.empty())
	{
		ModuleValueStatEffect data = m_valueEffect.back();
		m_valueEffect.pop_back();
	}
	// module health effect
	while(!m_healthEffects.empty())
	{
		ModuleHealthStatEffect data = m_healthEffects.back();
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

void ModuleStatusEffectSystem::addParticleEffectComponent( Entity* p_entity )
{
	if (p_entity->getComponent(ComponentType::ParticleSystemsComponent)==NULL)
	{
		ParticleSystemsComponent* particleEmitters = new ParticleSystemsComponent();
		// World highlight, always on top
		// =============================================
		AglParticleSystem particleSystem1;
		AglVector3 offset = AglVector3( 0.0f,0.0f,0.0f);
		particleSystem1.setParticleSize(AglVector2(30.0f,30.0f));
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
		particleEmitters->addParticleSystemInstruction(particleInstruction1);

		// art deco 1
		// =============================================
		AglParticleSystem particleSystem2;
		particleSystem2.setParticleSize(AglVector2(2.0f,2.0f));
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
		particleSystem2.setSpawnOffset(3.0f);
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
		particleEmitters->addParticleSystemInstruction(particleInstruction2);
		// art deco 2
		// =============================================
		AglParticleSystem particleSystem3;
		particleSystem3.setParticleSize(AglVector2(1.0f,1.0f));
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
		particleSystem3.setSpawnOffset(2.0f);
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
		particleEmitters->addParticleSystemInstruction(particleInstruction3);
		// art deco 3
		// =============================================
		AglParticleSystem particleSystem4;
		particleSystem4.setParticleSize(AglVector2(8.0f,8.0f));
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
		particleSystem4.setSpawnOffset(5.0f);
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
		particleEmitters->addParticleSystemInstruction(particleInstruction4);
		// art deco 4
		// =============================================
		AglParticleSystem particleSystem5;
		particleSystem5.setParticleSize(AglVector2(2.0f,2.0f));
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
		particleSystem5.setSpawnOffset(10.0f);
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
		particleEmitters->addParticleSystemInstruction(particleInstruction5);
		// art deco 5
		// =============================================
		AglParticleSystem particleSystem6;
		particleSystem6.setParticleSize(AglVector2(2.0f,2.0f));
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
		particleSystem6.setSpawnOffset(5.0f);
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
		particleEmitters->addParticleSystemInstruction(particleInstruction6);
		// art deco 7
		// =============================================
		AglParticleSystem particleSystem7;
		particleSystem7.setParticleSize(AglVector2(14.0f,14.0f));
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
		particleSystem7.setSpawnOffset(1.0f);
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
		particleEmitters->addParticleSystemInstruction(particleInstruction7);

		// add it
		p_entity->addComponent( ComponentType::ParticleSystemsComponent, particleEmitters );
		p_entity->applyComponentChanges();
	}
	
}

void ModuleStatusEffectSystem::removeParticleEffectComponent( Entity* p_entity )
{
	if (p_entity->getComponent(ComponentType::ParticleSystemsComponent)!=NULL)
	{
		p_entity->removeComponent(ComponentType::ParticleSystemsComponent);
		p_entity->applyComponentChanges();
	}
}
