// =======================================================================================
//                                      AntTweakBar
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A wrapper class around Ant Tweak Bar
///        
/// # AntTweakBar
/// Detailed description.....
/// Created on: 3-12-2012 
///---------------------------------------------------------------------------------------
#pragma once
#include <AntTweakBar.h>
#include <d3d11.h>

using namespace std;

class AntTweakBarWrapper
{
public:
	AntTweakBarWrapper(ID3D11Device* p_device, const char* p_nameOfBar);
	virtual ~AntTweakBarWrapper();
	TwBar* getAntBar();
	void render();
private:
	TwBar* m_antBar;
};