#include "Window.h"
#include "AntTweakBarWrapper.h"
#include "WindowException.h"
#include <DebugUtil.h>

Window* Window::m_instance=NULL;

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
	wcex.lpszClassName  = "Amalgamation";
	wcex.hIconSm        = 0;

	if( !RegisterClassEx(&wcex) )
	{
		throw WindowException();
	}

	// Create window
	RECT rc = { 0, 0, p_width, p_height};
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	if(!(m_hWnd = CreateWindow(
		"Amalgamation",
		"Amalgamation",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		m_hInstance,
		NULL)))
	{
		throw WindowException();
	}
	m_autoResize=false;
	m_sizeIsDirty=false;
	m_shutDownRequest=false;
	m_isFullscreen=false;
	m_instance=this;
	ShowWindow( m_hWnd, p_showWindowFlag );
	lockMouse();
}

Window::~Window()
{
	DestroyWindow(m_hWnd);
	//UnregisterClass("Amalgamation", m_hInstance);
}

HWND Window::getWindowRef()
{
	return m_hWnd;
}

void Window::changeWindowRes( int p_width, int p_height )
{
	m_width=p_width;
	m_height=p_height;

	SetWindowPos( m_hWnd, HWND_TOP, 0, 0, p_width, p_height,
		SWP_NOMOVE );
}

void Window::resize( int p_width, int p_height )
{
	if (p_width>1 && p_height>1)
	{
		m_width=p_width;
		m_height=p_height;
		m_sizeIsDirty=true;
	}
}

bool Window::isDirty()
{
	bool res = m_sizeIsDirty;
	m_sizeIsDirty=false;
	return res;
}

void Window::lockMouse()
{
	RECT clientRect;
	GetClientRect(m_hWnd,&clientRect);
	POINT p1 = {clientRect.left,clientRect.top};
	POINT p2 = {clientRect.right,clientRect.bottom};
	ClientToScreen(m_hWnd,&p1); ClientToScreen(m_hWnd,&p2);
	RECT bounds={p1.x,p1.y,p2.x,p2.y};
	ClipCursor(&bounds);
}

void Window::setAutoResize( bool p_mode )
{
	m_autoResize=p_mode;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	static int cursorIsHidden=ShowCursor(false);

	bool handled = false;

	Window* window = Window::getInstance();
	// anttweakbar is handled by dinput in fullscreen as 
	// cooperation is changed to exclusive then to allow for multimonitor
	if (window && !window->m_isFullscreen) 
	{
		handled = AntTweakBarWrapper::getInstance()->handleMessage(hWnd,message,wParam,lParam);
	}

	CURSORINFO cursorInfo;
	GetCursorInfo(&cursorInfo);

	RECT clientRect;
	GetClientRect(hWnd,&clientRect);


	TRACKMOUSEEVENT mouseEventTracker;
	TrackMouseEvent(&mouseEventTracker);


	if (!handled)
	{
		switch (message) 
		{
		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);

				break;
			}


		case WM_DESTROY:			
			{
				PostQuitMessage(0);
				Window* window = Window::getInstance();
				if (window)
				{
					window->requestShutDown();
				}
				break;
			}

		case WM_NCMOUSELEAVE:
		{
			// if (cursorHidden) ShowCursor(true);
			TrackMouseEvent(&mouseEventTracker);
		}

		case WM_NCMOUSEMOVE:
			{
				// if (cursorHidden) ShowCursor(true);
				TrackMouseEvent(&mouseEventTracker);
			}


				
		case WM_NCMOUSEHOVER:
			{
				/*POINT mousepos;
				GetCursorPos(&mousepos);
				ScreenToClient(hWnd,&mousepos);
				if (!(mousepos.x<=clientRect.right  &&
					mousepos.x>=clientRect.left 	 &&
					mousepos.y>=clientRect.top 	 &&
					mousepos.y<=clientRect.bottom))
				{*/
				if (cursorIsHidden<0)
				{
					cursorIsHidden=ShowCursor(true);
				}
				//}
			}

			break;


		case WM_SIZE:
			{
				Window* window = Window::getInstance();
				if (window && window->isAutoResizeEnabled())
				{
					window->resize(LOWORD(lParam),HIWORD(lParam));
				}
			}
			break;

		case WM_KEYDOWN:
			switch(wParam)
			{
			case VK_ESCAPE:
				//PostQuitMessage(0);
				break;
			}
			// fallthrough:
		case WM_SYSKEYDOWN:
			if (wParam == VK_RETURN)
			{
				if ((HIWORD(lParam) & KF_ALTDOWN))
				{
					Window* window = Window::getInstance();
					if (window)
					{
						window->m_isFullscreen=!window->m_isFullscreen;
					}
				}
			}
			break;

// 		case WM_MOUSEMOVE:
// 			{
// 				if (cursorIsHidden<0) // constrain even during fullscreen mode
// 				{
// 					POINT mousepos;
// 					GetCursorPos(&mousepos);
// 					GetClientRect(hWnd,&clientRect);
// 					if (mousepos.x >= clientRect.right) mousepos.x=clientRect.right-1;
// 					if (mousepos.x <= clientRect.left) mousepos.x=clientRect.left+1;
// 					if (mousepos.y >= clientRect.bottom) mousepos.y=clientRect.bottom-1;
// 					if (mousepos.y <= clientRect.top) mousepos.y=clientRect.top+1;
// 				}
// 			}

		case WM_LBUTTONDOWN:
			{
				while (cursorIsHidden>=0)
				{
					POINT mousepos;
					GetCursorPos(&mousepos);
					GetClientRect(hWnd,&clientRect);
					POINT p1 = {clientRect.left,clientRect.top};
					POINT p2 = {clientRect.right,clientRect.bottom};
					ClientToScreen(hWnd,&p1); ClientToScreen(hWnd,&p2);
					RECT bounds={p1.x,p1.y,p2.x,p2.y};
					if (mousepos.x<bounds.right  &&
						mousepos.x>bounds.left 	 &&
						mousepos.y>bounds.top 	 &&
						mousepos.y<bounds.bottom)
					{					
						cursorIsHidden=ShowCursor(false);
						Window* window = Window::getInstance();
						if (window)
						{
							window->lockMouse();
						}
					}
				}
			}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}


	return 0;
}