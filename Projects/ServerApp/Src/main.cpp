#include "ServerApplication.h"

int main(int argc, char** argv)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	ServerApplication* application;
	application = new ServerApplication();

	application->run();

	delete application;

	return 0;
}

