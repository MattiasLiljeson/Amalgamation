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
		m_unusedModuleEffects.pop_back();
	}
	// unused module effect
	while(!m_valueEffect.empty())
	{
		ModuleValueStatEffect data = m_valueEffect.back();
		m_valueEffect.pop_back();
	}
	// unused module effect
	while(!m_healthEffects.empty())
	{
		ModuleHealthStatEffect data = m_healthEffects.back();
		m_unusedModuleEffects.pop_back();
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
