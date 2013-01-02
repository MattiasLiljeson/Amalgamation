#include "ServerApplication.h"
#include <vld.h>

int main(int argc, char** argv)
{
	Srv::ServerApplication* application;
	application = new Srv::ServerApplication();

	application->run();

	delete application;

	return 0;
}

