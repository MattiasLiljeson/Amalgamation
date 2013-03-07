#include "ModuleStatusVisualizationMode.h"

ModuleStatusVisualizationMode::ModuleStatusVisualizationMode()
	: Component( ComponentType::ModuleStatusVisualizationMode )
{
	positionHintParticleSysId=-1;
}

ModuleStatusVisualizationMode::~ModuleStatusVisualizationMode()
{

}

bool ModuleStatusVisualizationMode::hasPositionHintParticle()
{
	return positionHintParticleSysId!=-1;
}

bool ModuleStatusVisualizationMode::hasUnusedHintParticles()
{
	return unusedHintParticleSysId.size()>0;
}

bool ModuleStatusVisualizationMode::hasHealthParticles()
{
	return healthParticlesSysId.size()>0;
}
