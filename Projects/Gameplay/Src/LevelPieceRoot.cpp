#include "LevelPieceRoot.h"

ComponentRegister<LevelPieceRoot> LevelPieceRoot::s_reg("LevelPieceRoot");

LevelPieceRoot::LevelPieceRoot()
	: Component(ComponentType::LevelPieceRoot)
{
	pieceId = -1;
	boundingSphere.radius = -1; //Invalid bounding sphere
	boundingSphere.position = AglVector3(0, 0, 0);
	shouldCull = false;
}
