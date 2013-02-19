//#include <vld.h>
#include "ClientApplication.h"
#include <DebugUtil.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow)
{
	ClientApplication* application;
	try{
		application = new ClientApplication( hInstance );
	}
	catch(exception& e){
		DEBUGWARNING((e.what()));
		return -1;
	}

	SetThreadAffinityMask(GetCurrentThread(), 1);
	application->run();

	delete application;

	//UINT32 numLeaks = VLDGetLeaksCount();

	/*if(numLeaks > 1233){
		MessageBoxA(NULL, "MEMORY LEAK DETECTED!!", "Warning!", MB_ICONWARNING);
	}*/

	return 0;
}