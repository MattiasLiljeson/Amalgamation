#include "LevelPieceRoot.h"

ComponentRegister<LevelPieceRoot> LevelPieceRoot::s_reg("LevelPieceRoot");

LevelPieceRoot::LevelPieceRoot()
	: Component(ComponentType::LevelPieceRoot)
{
}

LevelPieceRoot::LevelPieceRoot(const AglMatrix& p_initTransformMatrix)
	: Component(ComponentType::LevelPieceRoot)
{
	initTransformMatrix = p_initTransformMatrix;
}
