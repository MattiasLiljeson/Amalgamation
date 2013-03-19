#pragma once

#include <Component.h>
#include <ComponentFactory.h>

// =======================================================================================
//                                ModuleStatusVisualizationMode
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A client side component for keeping track of the status 
/// visualization of a module. For example what particle system id:s are reserved for 
/// the status effect so that they can be updated or removed by a system.
///        
/// # ModuleStatusVisualizationMode
/// Detailed description.....
/// Created on: 1-3-2013 
///---------------------------------------------------------------------------------------

class ModuleStatusVisualizationMode : public Component
{
public:
	ModuleStatusVisualizationMode();
	virtual ~ModuleStatusVisualizationMode();

	bool hasPositionHintParticle();
	bool hasHealthParticles();
	bool hasUnusedHintParticles();

	int positionHintParticleSysId;
	vector<int> healthParticlesSysId;
	vector<int> unusedHintParticleSysId;
protected:
private:

};