#pragma once

#include <Entity.h>
#include <EntitySystem.h>
#include <AglMatrix.h>
#include "EntityFactory.h"

using namespace std;

class ModuleStatusVisualizationMode;

// =======================================================================================
//                                ModuleStatusEffectSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ModuleStatusEffectSystem
/// Detailed description.....
/// Created on: 27-2-2013 
///---------------------------------------------------------------------------------------

class ModuleStatusEffectSystem : public EntitySystem
{
public:
	struct ModuleUnusedEffect
	{
		bool mode;
		Entity* moduleEntity;
		int value;
	};

	struct ModuleValueStatEffect
	{
		Entity* moduleEntity;
		float value;
	};

	struct ModuleHealthStatEffect
	{
		Entity* moduleEntity;
		float health;
	};	
	
	struct ModuleFreefloatEffect
	{
		bool mode;
		Entity* moduleEntity;
	};
	ModuleStatusEffectSystem();
	virtual ~ModuleStatusEffectSystem();
	
	void clear();

	virtual void initialize();
	virtual void process();

	void setUnusedModuleEffect(ModuleUnusedEffect& p_fx);
	void setValueEffect(ModuleValueStatEffect& p_fx);
	void setHealthEffect(ModuleHealthStatEffect& p_fx);
	void setFreefloatEffect(ModuleFreefloatEffect& p_fx);
protected:
private:
	vector<ModuleUnusedEffect> m_unusedModuleEffects;
	vector<ModuleValueStatEffect> m_valueEffect;
	vector<ModuleHealthStatEffect> m_healthEffects;
	vector<ModuleFreefloatEffect> m_freefloatEffects;

	// Particle effects
	// For unused modules
	void activateUpdateUnusedModuleParticleEffect(Entity* p_entity, ParticleSystemsComponent* p_ps,
		ModuleStatusVisualizationMode* p_visMode);
	ParticleSystemsComponent* addAndRegisterUnusedModuleParticleEffect(Entity* p_entity, 
		ModuleStatusVisualizationMode* p_visMode);
	void disableUnusedModuleParticleEffect(Entity* p_entity, ParticleSystemsComponent* p_ps,
		ModuleStatusVisualizationMode* p_visMode);

	// For hurt modules
	void activateUpdateDamagedModuleParticleEffect(Entity* p_entity, ParticleSystemsComponent* p_ps, ModuleStatusVisualizationMode* p_visMode, float p_health);
	ParticleSystemsComponent* addAndRegisterDamagedModuleParticleEffect(Entity* p_entity, 
		ModuleStatusVisualizationMode* p_visMode);
	void disableDamagedParticleEffect(Entity* p_entity, ParticleSystemsComponent* p_ps,
		ModuleStatusVisualizationMode* p_visMode);

	// For module value

	void updateFreefloatGlowEffect(Entity* p_entity, bool p_mode);
	float getDistSqBetweenShipAndModule(Entity* p_moduleEntity);

	Entity* m_myship;
	AglVector3 m_moduleColorTone;
};
