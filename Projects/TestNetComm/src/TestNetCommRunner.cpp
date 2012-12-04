#include <iostream>
#include <igloo/igloo_alt.h>

using namespace igloo;
using namespace std;

int main()
{
	VisualStudioResultsOutput output;
	TestRunner runner( output );

	int result = runner.Run();

	cin.get();

	return result;
}