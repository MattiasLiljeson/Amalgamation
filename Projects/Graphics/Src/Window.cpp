#include "Window.h"
#include "AntTweakBarWrapper.h"
#include "WindowException.h"

Window::Window(HINSTANCE p_hInstance, int p_width, int p_height, int p_showWindowFlag)
{
	m_hInstance = p_hInstance; 

	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc; // Callback function
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = m_hInstance;
	wcex.hIcon          = 0;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = "Windos 32";
	wcex.hIconSm        = 0;

	if( !RegisterClassEx(&wcex) )
		throw WindowException();

	// Create window
	RECT rc = { 0, 0, p_width, p_height};
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	if(!(m_hWnd = CreateWindow(
		"Windos 32",
		"Windos 32",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		m_hInstance,
		NULL)))
	{
		throw WindowException();
	}

	ShowWindow( m_hWnd, p_showWindowFlag );
}

Window::~Window()
{

}

HWND Window::getWindowRef()
{
	return m_hWnd;
}

void Window::changeWindowRes( int p_width, int p_height )
{
	SetWindowPos( m_hWnd, HWND_TOP, 0, 0, p_width, p_height,
		SWP_NOMOVE );
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	AntTweakBarWrapper::getInstance()->handleMessage(hWnd,message,wParam,lParam);

	switch (message) 
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}