#pragma once

#include <Component.h>
#include <AglVector4.h>
#include <ComponentFactory.h>

// =======================================================================================
//                                 EnvironmentVariable
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Environment
///        
/// # EnvironmentValues
/// Detailed description.....
/// Created on: 19-2-2013 
///---------------------------------------------------------------------------------------

class EnvironmentValues : public Component
{
public:
	EnvironmentValues();
	virtual ~EnvironmentValues();

	AglVector4 fogColor;
	AglVector4 ambientColor;
	float radius; ///< radius range of environment values
	float fogFarPlaneClosenessPercentage; ///< closeness of fog end-gradient to far plane
	float fogNearPlaneClosenessPercentage; ///< closeness of fog start-gradient to near plane
	void init( vector<ComponentData> p_initData );

private:

	static ComponentRegister<EnvironmentValues> s_reg;
};