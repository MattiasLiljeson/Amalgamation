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
	ModuleStatusEffectSystem();
	virtual ~ModuleStatusEffectSystem();
	
	virtual void initialize();
	virtual void process();

	void setUnusedModuleEffect(ModuleUnusedEffect& p_fx);
	void setValueEffect(ModuleValueStatEffect& p_fx);
	void setHealthEffect(ModuleHealthStatEffect& p_fx);
protected:
private:
	vector<ModuleUnusedEffect> m_unusedModuleEffects;
	vector<ModuleValueStatEffect> m_valueEffect;
	vector<ModuleHealthStatEffect> m_healthEffects;

	void activateUpdateParticleEffect(Entity* p_entity,
		ModuleStatusVisualizationMode* p_visMode);
	void addAndRegisterParticleEffect(Entity* p_entity, 
									  ModuleStatusVisualizationMode* p_visMode);
	void disableParticleEffect(Entity* p_entity, 
		ModuleStatusVisualizationMode* p_visMode);

	Entity* m_myship;
	AglVector3 m_moduleColorTone;
};
