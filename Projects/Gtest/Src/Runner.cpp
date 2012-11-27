#include <gtest\gtest.h>
#include <iostream>

int main(int argc, char **argv)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	::testing::InitGoogleTest(&argc, argv);

	int success = RUN_ALL_TESTS();

	std::cin.get();

	return success;
}