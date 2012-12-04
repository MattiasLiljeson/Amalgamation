// =======================================================================================
//                                      AntTweakBar
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A wrapper class around Ant Tweak Bar utilizing the singleton pattern
///        
/// # AntTweakBar
/// This class contains some restrictions and should be used with caution. Please note 
/// that the getInstance function with parameters should always be called once in init
/// stage. 
/// Created on: 3-12-2012 
///---------------------------------------------------------------------------------------
#pragma once
#include <AntTweakBar.h>
#include <d3d11.h>

class AntTweakBarWrapper
{
public:
	///-----------------------------------------------------------------------------------
	/// This function should only be called with parameters once to init Ant Tweak Bar 
	/// Wrapper(ATBW) properly. Afterwards just call the function without any parameters
	/// \param p_device
	/// \param p_nameOfBar
	/// \returns AntTweakBarWrapper*
	///-----------------------------------------------------------------------------------
	static AntTweakBarWrapper* getInstance(ID3D11Device* p_device=NULL, 
		const char* p_nameOfBar=NULL);

	///-----------------------------------------------------------------------------------
	/// Called to destroy the ATBW
	/// \returns void
	///-----------------------------------------------------------------------------------
	static void destroy();

	void	addReadOnlyVariable(const char* p_name, TwType p_type, const void *p_var, 
		const char* p_misc);

	void	addWriteVariable(const char* p_name, TwType p_type, void *p_var,
		const char* p_misc);

	void	render();
	void	handleMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	AntTweakBarWrapper(ID3D11Device* p_device, const char* p_nameOfBar);
	virtual ~AntTweakBarWrapper();
private:
	TwBar* m_antBar;
	static AntTweakBarWrapper* sInstance;
};