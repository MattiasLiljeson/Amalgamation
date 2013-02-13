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
	LevelPieceInfo(int p_pieceInstanceId)
		: Component(ComponentType::LevelPieceInfo)
	{
		m_pieceInstanceId = p_pieceInstanceId;
	}
	int getPieceInstanceId() const
	{
		return m_pieceInstanceId;
	}
private:
	int m_pieceInstanceId;
};