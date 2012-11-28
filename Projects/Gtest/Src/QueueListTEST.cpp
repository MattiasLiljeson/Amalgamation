#include <gtest\gtest.h>
#include "QueueList.h"

TEST(QueueListPushBack, CheckLength)
{
	QueueList<int> list;
	for(int i = 0; i < 10; i++)
	{
		list.pushBack(i + 5);
	}

	EXPECT_EQ(10, list.length());
}


TEST(QueueListPopBack, PopAfterAllPushBacks)
{
	QueueList<int> list;
	for(int i = 0; i < 10; i++)
	{
		list.pushBack(i + 5);
	}

	ASSERT_EQ(10, list.length());

	for(int i = 0; i < 10; i++)
	{
		EXPECT_EQ(i + 5, list.popFront());
	}
}

TEST(QueueListPopBack, PopAfterEachPushBack)
{
	QueueList<int> list;
	for(int i = 0; i < 10; i++)
	{
		list.pushBack(i + 5);
		EXPECT_EQ(i + 5, list.popFront());
	}

	EXPECT_EQ(0, list.length());
}

TEST(QueueBuildArray, FewElements)
{
	QueueList<int> list;
	for(int i = 0; i < 10; i++)
	{
		list.pushBack(i);
	}

	ASSERT_EQ(10, list.length());

	int* buffer = new int[list.length()];
	list.buildArray(buffer);

	for(int i = 0; i < 10; i++)
	{
		EXPECT_EQ(i, buffer[i]);
	}
	delete[] buffer;
}