#pragma once

// Will only print in debug, will replace call in release with "nothing"
// call like this: DEBUGPRINT(("text"));
// #ifdef _DEBUG
	static void debugPrint(const char* msg);
	#define DEBUGPRINT(x) debugPrint x
	#ifdef WINAPI
		#include <Windows.h>
		void debugPrint(const char* msg)
		{
			OutputDebugStringA(msg);
		}
	#else
		#ifdef FORCE_VS_DBG_OUTPUT
			#include <Windows.h>
			void debugPrint(const char* msg)
			{
				OutputDebugStringA(msg);
			}
		#else
			#include <iostream>
			void debugPrint(const char* msg)
			{
				std::cout<<msg;
			}
		#endif
	#endif
// #else
// 	#define DEBUGPRINT(x)
// #endif

// Warning version
// #ifdef _DEBUG
static void debugWarn(const char* msg);
 #define DEBUGWARNING(x) debugWarn x

// #ifdef WINAPI  // commented this out to force popup
#include <Windows.h>
void debugWarn(const char* msg)
{
	OutputDebugStringA((msg));
	MessageBoxA(NULL, (msg), "Warning!", MB_ICONWARNING);
}
// #else
// #define DEBUGWARNING(x)
/*#endif*/