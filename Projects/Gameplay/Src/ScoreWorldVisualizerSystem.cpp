#include "ScoreWorldVisualizerSystem.h"
#include <DebugUtil.h>
#include <ToString.h>

ScoreWorldVisualizerSystem::ScoreWorldVisualizerSystem() : 
	EntitySystem( SystemType::ScoreWorldVisualizerSystem)
{

}

ScoreWorldVisualizerSystem::~ScoreWorldVisualizerSystem()
{

}

void ScoreWorldVisualizerSystem::initialize()
{

}

void ScoreWorldVisualizerSystem::process()
{
	while(!m_effectsToCreate.empty())
	{
		DEBUGWARNING((toString(m_effectsToCreate.back().score).c_str()));
		m_effectsToCreate.pop_back();
	}
}

void ScoreWorldVisualizerSystem::addEffect( ScoreEffectCreationData& p_fx )
{
	m_effectsToCreate.push_back(p_fx);
}
