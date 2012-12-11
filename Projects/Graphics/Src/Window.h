// =======================================================================================
//                                      Window
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # Window
/// Detailed description.....
/// Created on: 27-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <MessageLoopFetcher.h>

#include <Windows.h>
#include "AntTweakBarWrapper.h"
#include "WindowException.h"

static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

class Window
{
public:
	Window(HINSTANCE p_hInstance, int p_width, int p_height, int p_showWindowFlag);
	virtual ~Window();
	HWND getWindowRef();
protected:
private:
	int m_width;
	int m_height;
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
};