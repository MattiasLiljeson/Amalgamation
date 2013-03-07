#pragma once
#include <Component.h>
#include <ComponentFactory.h>
// =======================================================================================
// TeslaCoilEffect
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # TeslaCoilEffect
/// Detailed description...
/// Created on: 7-3-2013 
///---------------------------------------------------------------------------------------
class TeslaCoilEffect: public Component
{
public:
	TeslaCoilEffect();
	void init( vector<ComponentData> p_initData );

public:
	vector<string> possibleMeshes;

public:
	static ComponentRegister<TeslaCoilEffect> s_reg;
};