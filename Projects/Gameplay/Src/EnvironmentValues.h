#pragma once

#include <Component.h>
#include <AglVector3.h>
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

	AglVector3 m_fogColor;
	AglVector3 m_ambientColor;
	float m_minRadius; ///< falloff start of values
	float m_maxRadius; ///< maximum radius range of environment values ( falloff end)
	float m_fogFarPlaneClosenessPercentage; ///< closeness of fog end-gradient to far plane
	float m_fogNearPlaneClosenessPercentage; ///< closeness of fog start-gradient to near plane
	void init( vector<ComponentData> p_initData );

private:

	static ComponentRegister<EnvironmentValues> s_reg;
};