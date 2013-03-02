#include "TeslaEffectPiece.h"

TeslaEffectPiece::TeslaEffectPiece()
	: Component(ComponentType::TeslaEffectPiece)
{
	lifeTime = 0.0f;
}

TeslaEffectPiece::TeslaEffectPiece(float p_lifeTime)
	: Component(ComponentType::TeslaEffectPiece)
{
	lifeTime = p_lifeTime;
}