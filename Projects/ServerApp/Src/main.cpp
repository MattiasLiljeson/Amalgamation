#include "ServerApplication.h"
#include <vld.h>

int main(int argc, char** argv)
{
	std::ofstream matlabOut("matlab_out_pieces.m", std::ifstream::trunc);
	if (matlabOut.is_open())
	{
		matlabOut.close();
	}
	matlabOut = std::ofstream("matlab_out.m", std::ifstream::trunc);
	if (matlabOut.is_open())
	{
		matlabOut.close();
	}

	Srv::ServerApplication* application;
	application = new Srv::ServerApplication();

	application->body();

	delete application;

	return 0;
}