#include "AntTweakBarWrapper.h"

AntTweakBarWrapper* AntTweakBarWrapper::sInstance = NULL;

AntTweakBarWrapper::AntTweakBarWrapper( ID3D11Device* p_device)
{
	TwInit(TW_DIRECT3D11,p_device);
	m_overallBar	= TwNewBar(overall.c_str());
	m_soundBar		= TwNewBar(sound.c_str());
	m_graphicsBar	= TwNewBar(graphics.c_str());
	m_input			= TwNewBar(input.c_str());
	m_network		= TwNewBar(network.c_str());
}

AntTweakBarWrapper::~AntTweakBarWrapper()
{
	TwTerminate();
}
void AntTweakBarWrapper::render()
{
	TwDraw();
}

AntTweakBarWrapper* AntTweakBarWrapper::getInstance(ID3D11Device* p_device/* =NULL */)
{
	if (sInstance == NULL)
		if(p_device != NULL)
			sInstance = new AntTweakBarWrapper(p_device);
	return sInstance;
}

void AntTweakBarWrapper::destroy()
{
	if (sInstance)	
		delete sInstance;
}

void AntTweakBarWrapper::handleMessage( HWND hWnd, UINT message, WPARAM wParam, 
									   LPARAM lParam )
{
	if ( TwEventWin(hWnd, message, wParam, lParam) ) // send event message to AntTweakBar
		return; // event has been handled by AntTweakBar
}

void AntTweakBarWrapper::addReadOnlyVariable(Bar p_barType, const char* p_name, 
											 TwType p_type, const void *p_var, 
											 const char* p_misc)
{
	switch (p_barType)
	{
	case AntTweakBarWrapper::SOUND:
		TwAddVarRO(m_soundBar,p_name,p_type,p_var,p_misc);
		break;
	case AntTweakBarWrapper::GRAPHICS:
		TwAddVarRO(m_graphicsBar,p_name,p_type,p_var,p_misc);
		break;
	case AntTweakBarWrapper::INPUT:
		TwAddVarRO(m_input,p_name,p_type,p_var,p_misc);
		break;
	case AntTweakBarWrapper::NETWORK:
		TwAddVarRO(m_network,p_name,p_type,p_var,p_misc);
		break;
	default:
		TwAddVarRO(m_overallBar,p_name,p_type,p_var,p_misc);
		break;
	}
}

void AntTweakBarWrapper::addWriteVariable(Bar p_barType, const char* p_name, 
										  TwType p_type, void *p_var, 
										  const char* p_misc)
{
	switch (p_barType)
	{
	case AntTweakBarWrapper::SOUND:
		TwAddVarRW(m_soundBar,p_name,p_type,p_var,p_misc);
		break;
	case AntTweakBarWrapper::GRAPHICS:
		TwAddVarRW(m_graphicsBar,p_name,p_type,p_var,p_misc);
		break;
	case AntTweakBarWrapper::INPUT:
		TwAddVarRW(m_input,p_name,p_type,p_var,p_misc);
		break;
	case AntTweakBarWrapper::NETWORK:
		TwAddVarRW(m_network,p_name,p_type,p_var,p_misc);
		break;
	default:
		TwAddVarRW(m_overallBar,p_name,p_type,p_var,p_misc);
		break;
	}
}

TwBar* AntTweakBarWrapper::getAntBar(Bar p_barType)
{
	TwBar* searchedBar = NULL;
	
	switch (p_barType)
	{
	case AntTweakBarWrapper::SOUND:
		searchedBar = m_soundBar;
		break;
	case AntTweakBarWrapper::GRAPHICS:
		searchedBar = m_graphicsBar;
		break;
	case AntTweakBarWrapper::INPUT:
		searchedBar = m_input;
		break;
	case AntTweakBarWrapper::NETWORK:
		searchedBar = m_network;
		break;
	default:
		searchedBar = m_overallBar;
		break;
	}

	return searchedBar;
}
