#include "TeslaEffectPiece.h"

ComponentRegister<TeslaEffectPiece> TeslaEffectPiece::s_reg("TeslaEffectPiece");

TeslaEffectPiece::TeslaEffectPiece()
	: Component(ComponentType::TeslaEffectPiece)
{
	lifeTime = 0.0f;
	maxLifeTime = 0.0f;
	damages = false;
}

TeslaEffectPiece::TeslaEffectPiece(AglVector3 p_forwardScale)
	: Component(ComponentType::TeslaEffectPiece)
{
	lifeTime = 0.0f;
	maxLifeTime = 0.0f;
	forwardScale = p_forwardScale;
	damages = false;
}

TeslaEffectPiece::TeslaEffectPiece(float p_lifeTime, AglVector3 p_forwardScale)
	: Component(ComponentType::TeslaEffectPiece)
{
	lifeTime = 0.0f;
	maxLifeTime = p_lifeTime;
	forwardScale = p_forwardScale;
	damages = false;
}

void TeslaEffectPiece::init( vector<ComponentData> p_initData )
{
	string possibleMesh = "";
	for(unsigned int i=0; i<p_initData.size(); i++)
	{
		if(p_initData[i].dataName == "lifeTime") {
			p_initData[i] >> maxLifeTime;
		}
	}
}