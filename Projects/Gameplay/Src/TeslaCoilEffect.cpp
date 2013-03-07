#include "TeslaCoilEffect.h"

ComponentRegister<TeslaCoilEffect> TeslaCoilEffect::s_reg("TeslaCoilEffect");

TeslaCoilEffect::TeslaCoilEffect()
	: Component(ComponentType::TeslaCoilEffect)
{
}

void TeslaCoilEffect::init( vector<ComponentData> p_initData )
{
	string possibleMesh = "";
	for(unsigned int i=0; i<p_initData.size(); i++)
	{
		if(p_initData[i].dataName == "possibleMesh") {
			p_initData[i] >> possibleMesh;
			possibleMeshes.push_back(possibleMesh);
		}
	}
}