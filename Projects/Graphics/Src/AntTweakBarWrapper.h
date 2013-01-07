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
	enum BarType
	{
		OVERALL, SOUND, GRAPHICS, INPUT, NETWORK
	};
public:
	///-----------------------------------------------------------------------------------
	/// This function should only be called with parameters once to init Ant Tweak Bar 
	/// Wrapper(ATBW) properly. Afterwards just call the function without any parameters
	/// \param p_device
	/// \return AntTweakBarWrapper*
	///-----------------------------------------------------------------------------------
	static AntTweakBarWrapper* getInstance(ID3D11Device* p_device=NULL);

	///-----------------------------------------------------------------------------------
	/// Called to destroy the ATBW
	/// \return void
	///-----------------------------------------------------------------------------------
	static void destroy();

	///-----------------------------------------------------------------------------------
	/// Takes a variable and maps it to the specified bar allowing for AntTweakBar to only
	/// read it. 
	/// \param p_barType
	/// \param p_name
	/// \param p_type
	/// \param p_var
	/// \param p_misc
	/// \return void
	///-----------------------------------------------------------------------------------
	void addReadOnlyVariable(BarType p_barType, const char* p_name, TwType p_type, 
		const void *p_var, const char* p_misc);

	///-----------------------------------------------------------------------------------
	/// Takes a variable and maps it to the specified bar allowing for AntTweakBar to read
	/// and write to it. 
	/// \param p_barType
	/// \param p_name
	/// \param p_type
	/// \param p_var
	/// \param p_misc
	/// \return void
	///-----------------------------------------------------------------------------------
	void addWriteVariable(BarType p_barType, const char* p_name, TwType p_type, void *p_var,
		const char* p_misc);

	///-----------------------------------------------------------------------------------
	/// Tells AntTweakBar to be rendered
	/// \return void
	///-----------------------------------------------------------------------------------
	void render();

	///-----------------------------------------------------------------------------------
	/// Receives the messages sent from the windows message loop
	/// \param wnd
	/// \param msg
	/// \param wParam
	/// \param lParam
	/// \return void
	///-----------------------------------------------------------------------------------
	void handleMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

	///-----------------------------------------------------------------------------------
	/// Returns the specified AntTweakBar
	/// \param p_barType
	/// \return TwBar*
	///-----------------------------------------------------------------------------------
	TwBar* getAntBar(BarType p_barType);

	///-----------------------------------------------------------------------------------
	/// Allows for modification on the refresh-rate for any Bar, negative values are not
	/// a valid input.
	/// \param p_barType
	/// \param p_value
	/// \return void
	///-----------------------------------------------------------------------------------
	void modifyTheRefreshRate(BarType p_barType, float p_value);

private:
	///-----------------------------------------------------------------------------------
	/// The constructor is private since ATBW is a singleton class
	/// \param p_device
	/// \return 
	///-----------------------------------------------------------------------------------
	AntTweakBarWrapper(ID3D11Device* p_device);
	virtual ~AntTweakBarWrapper();

	///-----------------------------------------------------------------------------------
	/// Initialize the values and parameters for all of the Bars.
	/// \return void
	///-----------------------------------------------------------------------------------
	void initBars();

	///-----------------------------------------------------------------------------------
	/// A help method that allows for easy addition of defines for a given Bar.
	/// \param p_nameOfBar
	/// \param p_params
	/// \return void
	///-----------------------------------------------------------------------------------
	void defineParametersForABar(const char* p_nameOfBar, const char* p_params);
private:
	TwBar* m_overallBar;
	TwBar* m_soundBar;
	TwBar* m_graphicsBar;
	TwBar* m_input;
	TwBar* m_network;

	static AntTweakBarWrapper* sInstance;
};