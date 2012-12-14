#include "AntTweakBarWrapper.h"

AntTweakBarWrapper* AntTweakBarWrapper::sInstance = NULL;

AntTweakBarWrapper::AntTweakBarWrapper( ID3D11Device* p_device, const char* p_nameOfBar )
{
	TwInit(TW_DIRECT3D11,p_device);
	m_antBar = TwNewBar(p_nameOfBar);
}

AntTweakBarWrapper::~AntTweakBarWrapper()
{
	TwTerminate();
}
void AntTweakBarWrapper::render()
{
	TwDraw();
}

AntTweakBarWrapper* AntTweakBarWrapper::getInstance(ID3D11Device* p_device/* =NULL */, 
													const char* p_nameOfBar/* =NULL */)
{
	if (sInstance == NULL)
		if(p_device != NULL && p_nameOfBar != NULL)
			sInstance = new AntTweakBarWrapper(p_device,p_nameOfBar);
	
	return sInstance;
}

void AntTweakBarWrapper::destroy()
{
	if (sInstance)	
		delete sInstance;
}

void AntTweakBarWrapper::handleMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if ( TwEventWin(hWnd, message, wParam, lParam) ) // send event message to AntTweakBar
		return; // event has been handled by AntTweakBar
}

void AntTweakBarWrapper::addReadOnlyVariable( const char* p_name, TwType p_type, 
											 const void *p_var, const char* p_misc )
{
	TwAddVarRO(m_antBar,p_name,p_type,p_var,p_misc);
}

void AntTweakBarWrapper::addWriteVariable(const char* p_name, TwType p_type, 
										  void *p_var, const char* p_misc)
{
	TwAddVarRW(m_antBar,p_name,p_type,p_var,p_misc);
}

TwBar* AntTweakBarWrapper::getMainBar()
{
	return m_antBar;
}