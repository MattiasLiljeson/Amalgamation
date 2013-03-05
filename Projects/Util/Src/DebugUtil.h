#pragma once

#include "DebugFileWriter.h"

static void printToFile(const char* msg)
{
	static DebugFileWriter debugWriter;
	debugWriter.writeToFile(msg);
}

// Will only print in debug, will replace call in release with "nothing"
// call like this: DEBUGPRINT(("text"));
// #ifdef _DEBUG
	static void debugPrint(const char* msg);
  #ifdef _DEBUG 
	#define DEBUGPRINT(x) debugPrint x
  #else
	#define DEBUGPRINT(x) printToFile x
  #endif
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
#ifdef _DEBUG 
  #define DEBUGWARNING(x) debugWarn x
#else
  #define DEBUGWARNING(x) printToFile x
#endif
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