#include <vld.h>
#include "ClientApplication.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow)
{
	ClientApplication* application;
	application = new ClientApplication( hInstance );

	SetThreadAffinityMask(GetCurrentThread(), 1);

	application->run();

	delete application;

	return 0;
}