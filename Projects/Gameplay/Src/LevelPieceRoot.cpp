#include "LevelPieceRoot.h"

ComponentRegister<LevelPieceRoot> LevelPieceRoot::s_reg("LevelPieceRoot");

LevelPieceRoot::LevelPieceRoot()
	: Component(ComponentType::LevelPieceRoot)
{
	pieceId = -1;
}
