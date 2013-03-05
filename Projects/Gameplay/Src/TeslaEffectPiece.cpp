#include "TeslaEffectPiece.h"

ComponentRegister<TeslaEffectPiece> TeslaEffectPiece::s_reg("TeslaEffectPiece");

TeslaEffectPiece::TeslaEffectPiece()
	: Component(ComponentType::TeslaEffectPiece)
{
	lifeTime = 0.0f;
}

TeslaEffectPiece::TeslaEffectPiece(AglVector3 p_forwardScale)
	: Component(ComponentType::TeslaEffectPiece)
{
	lifeTime = 0.0f;
	forwardScale = p_forwardScale;
}

TeslaEffectPiece::TeslaEffectPiece(float p_lifeTime, AglVector3 p_forwardScale)
	: Component(ComponentType::TeslaEffectPiece)
{
	lifeTime = p_lifeTime;
	forwardScale = p_forwardScale;
}

void TeslaEffectPiece::init( vector<ComponentData> p_initData )
{
	string possibleMesh = "";
	for(unsigned int i=0; i<p_initData.size(); i++)
	{
		if(p_initData[i].dataName == "lifeTime") {
			p_initData[i] >> lifeTime;
		}
		else if(p_initData[i].dataName == "possibleMesh") {
			p_initData[i] >> possibleMesh;
			possibleMeshes.push_back(possibleMesh);
		}
	}
}