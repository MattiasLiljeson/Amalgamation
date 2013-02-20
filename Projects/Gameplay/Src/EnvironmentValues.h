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

protected:
private:
};