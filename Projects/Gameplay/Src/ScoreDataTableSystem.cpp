#include "ScoreDataTableSystem.h"
#include <ComponentType.h>

ScoreDataTableSystem::ScoreDataTableSystem()
	: EntitySystem(SystemType::ScoreDataTableSystem,
	2, ComponentType::PlayerScore, ComponentType::NetworkSynced)
{

}



ScoreDataTableSystem::~ScoreDataTableSystem()
{

}
