#include <igloo/igloo_alt.h>
#include <conio.h>
#include "TestAudioEngine.h"

using namespace igloo;

int main()
{
	int i = TestRunner::RunAllTests();
	_getch();
	return i;
}
