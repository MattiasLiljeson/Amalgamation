#pragma once

#include <Component.h>

// =======================================================================================
//                                      LevelPieceInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LevelPieceInfo
/// Detailed description.....
/// Created on: 13-2-2013 
///---------------------------------------------------------------------------------------

class LevelPieceInfo : public Component
{
public:
	LevelPieceInfo()
		: Component(ComponentType::LevelPieceInfo)
	{
		pieceInstanceId = -1;
	}
	int pieceInstanceId;
};