#include "AntTweakBarWrapper.h"


AntTweakBarWrapper::AntTweakBarWrapper( ID3D11Device* p_device, const char* p_nameOfBar )
{
	TwInit(TW_DIRECT3D11, p_device);
	m_antBar = TwNewBar(p_nameOfBar);
}

AntTweakBarWrapper::~AntTweakBarWrapper()
{
	TwTerminate();
}

TwBar* AntTweakBarWrapper::getAntBar()
{
	return m_antBar; 
}

void AntTweakBarWrapper::render()
{
	TwDraw();
}
