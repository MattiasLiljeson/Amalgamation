#pragma once

#include <Component.h>
#include <AglMatrix.h>
#include <ComponentFactory.h>

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
	LevelPieceRoot(const AglMatrix& p_initTransformMatrix);

	AglMatrix initTransformMatrix;

private:
	static ComponentRegister<LevelPieceRoot> s_reg;
};