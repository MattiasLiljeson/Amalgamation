#include <igloo/igloo_alt.h>
#include <conio.h>
#include "TestResourceManager.h"

using namespace igloo;

int main()
{
	int i = TestRunner::RunAllTests();
	getch();
	return i;
}
	