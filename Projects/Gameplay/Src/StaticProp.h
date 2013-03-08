#pragma once

#include <Component.h>

enum StaticPropType
{
	STATICPROPTYPE_UNSPECIFIED,
	STATICPROPTYPE_LEVELPIECE,
	STATICPROPTYPE_PLUGPIECE
};

// =======================================================================================
//                                      StaticProp
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # StaticProp
/// Detailed description.....
/// Created on: 11-1-2013 
///---------------------------------------------------------------------------------------

class StaticProp : public Component
{
public:
	StaticProp(int p_meshInfo = -1, StaticPropType p_propType = STATICPROPTYPE_UNSPECIFIED)
		: Component( ComponentType::StaticProp )
	{ 
		meshInfo		= p_meshInfo;
		propType		= p_propType;
	};
	~StaticProp(){};

	int				meshInfo;
	StaticPropType	propType;
};