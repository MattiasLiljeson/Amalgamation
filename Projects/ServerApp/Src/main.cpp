#include "ServerApplication.h"
#include <vld.h>

int main(int argc, char** argv)
{
	std::ofstream outfile("matlab_out_pieces.m", std::ifstream::trunc);
	if (outfile.is_open())
	{
		outfile.close();
	}
	outfile = std::ofstream("matlab_out.m", std::ifstream::trunc);
	if (outfile.is_open())
	{
		outfile.close();
	}
	outfile = std::ofstream("levelgen_out_spheres.txt", std::ifstream::trunc);
	if (outfile.is_open())
	{
		outfile.close();
	}
	outfile = std::ofstream("levelgen_result_size_diameter.txt", std::ifstream::trunc);
	if (outfile.is_open())
	{
		outfile.close();
	}
	outfile = std::ofstream("levelgen_out_hiearchy.txt", std::ifstream::trunc);
	if (outfile.is_open())
	{
		outfile.close();
	}

	Srv::ServerApplication* application;
	application = new Srv::ServerApplication();

	application->body();

	delete application;

	return 0;
}