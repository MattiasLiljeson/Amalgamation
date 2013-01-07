#pragma once
#include <AntTweakBar.h>
#include <d3d11.h>
#include <string>

using namespace std;
namespace AntTweakBarStrings{
	static const string& overall	= "Overall";
	static const string& sound		= "Sound";
	static const string& graphics	= "Graphics";
	static const string& input		= "Input";
	static const string& network	= "Network";
}
using namespace AntTweakBarStrings;
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

class AntTweakBarWrapper
{
public:
	enum Bar
	{
		OVERALL, SOUND, GRAPHICS, INPUT, NETWORK
	};
public:
	///-----------------------------------------------------------------------------------
	/// This function should only be called with parameters once to init Ant Tweak Bar 
	/// Wrapper(ATBW) properly. Afterwards just call the function without any parameters
	/// \param p_device
	/// \param p_nameOfBar
	/// \return AntTweakBarWrapper*
	///-----------------------------------------------------------------------------------
	static AntTweakBarWrapper* getInstance(ID3D11Device* p_device=NULL);

	///-----------------------------------------------------------------------------------
	/// Called to destroy the ATBW
	/// \return void
	///-----------------------------------------------------------------------------------
	static void destroy();

	void	addReadOnlyVariable(Bar p_barType, const char* p_name, TwType p_type, 
		const void *p_var, const char* p_misc);

	void	addWriteVariable(Bar p_barType, const char* p_name, TwType p_type, void *p_var,
		const char* p_misc);

	void	render();
	void	handleMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
	TwBar*	getAntBar(Bar p_barType);
private:
	AntTweakBarWrapper(ID3D11Device* p_device);
	virtual ~AntTweakBarWrapper();
private:
	TwBar* m_overallBar;
	TwBar* m_soundBar;
	TwBar* m_graphicsBar;
	TwBar* m_input;
	TwBar* m_network;

	static AntTweakBarWrapper* sInstance;
};