#pragma once
#include <Component.h>
#include <ComponentFactory.h>
// =======================================================================================
//	LightBlinker
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # LightBlinker
/// Detailed description...
/// Created on: 28-1-2013 
///---------------------------------------------------------------------------------------

class LightBlinker: public Component
{
public:
	LightBlinker();
	LightBlinker(float p_maxRange)
	{
		increase = true;
		maxRange = p_maxRange;
	}
	~LightBlinker(){}
	virtual void init( vector<ComponentData> p_initData );
public:
	bool increase;
	float maxRange;
private:
	static ComponentRegister<LightBlinker> s_reg;
};