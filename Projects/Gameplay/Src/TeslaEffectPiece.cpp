#include "TeslaEffectPiece.h"

TeslaEffectPiece::TeslaEffectPiece()
	: Component(ComponentType::TeslaEffectPiece)
{
	lifeTime = 0.0f;
}

TeslaEffectPiece::TeslaEffectPiece(float p_lifeTime, AglVector3 p_forwardScale)
	: Component(ComponentType::TeslaEffectPiece)
{
	lifeTime = p_lifeTime;
	forwardScale = p_forwardScale;
}