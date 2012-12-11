#include <iostream>
#include <igloo/igloo_alt.h>
#include <vld.h>

using namespace igloo;
using namespace std;

int main()
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	VisualStudioResultsOutput output;
	TestRunner runner( output );

	int result = runner.Run();

	cin.get();

	return result;
}