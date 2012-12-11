
#include "ClientApplication.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	ClientApplication* application;
	application = new ClientApplication( hInstance );

	application->run();

	delete application;

	return 0;
}