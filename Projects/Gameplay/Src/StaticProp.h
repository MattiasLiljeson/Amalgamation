#pragma once

#include <Component.h>

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
	StaticProp(int p_meshInfo = -1, bool p_isLevelPiece = false)
		: Component( ComponentType::StaticProp )
	{ 
		meshInfo		= p_meshInfo;
		isLevelPiece	= p_isLevelPiece;
	};
	~StaticProp(){};

	int		meshInfo;
	bool	isLevelPiece;
};