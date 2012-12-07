#pragma once

// Will only print in debug, will replace call in release with "nothing"
// call like this: DEBUGPRINT(("text"));
#ifdef _DEBUG
	static void debugPrint(const char* msg);
	#define DEBUGPRINT(x) debugPrint x
	#ifdef WINAPI
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
#else
	#define DEBUGPRINT(x)
#endif