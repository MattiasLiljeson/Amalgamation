#pragma once

#include <Component.h>
#include <AglMatrix.h>
#include <ComponentFactory.h>
#include <AglBoundingSphere.h>

// =======================================================================================
//                                      LevelPieceRoot
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LevelPieceRoot
/// Detailed description.....
/// Created on: 14-2-2013 
///---------------------------------------------------------------------------------------

class LevelPieceRoot : public Component
{
public:
	LevelPieceRoot();
	
	int pieceId;
	AglBoundingSphere boundingSphere;
	bool shouldCull;
private:
	static ComponentRegister<LevelPieceRoot> s_reg;
};