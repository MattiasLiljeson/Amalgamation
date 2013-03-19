#pragma once
#include <Component.h>
#include <ComponentFactory.h>
// =======================================================================================
// AnomalyAcceleratorModule
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief A module that accelerates large particles and crashes them together almost at
/// the speed of light. This in turn give rise to a chaotic pattern in the Higgs field
/// and creates tiny black holes that lasts for several seconds.
///        
/// # AnomalyAcceleratorModule
/// Created on: 26-2-2013 
///---------------------------------------------------------------------------------------
class AnomalyAcceleratorModule: public Component
{
public:
	AnomalyAcceleratorModule();
	void init( vector<ComponentData> p_initData );

public:
	float cooldown;
	float cooldownTime;
	float launchSpeed;
	bool owned;
	int currentBomb;
	float timeSinceLaunchStart;

public:
	static ComponentRegister<AnomalyAcceleratorModule> s_reg;
};