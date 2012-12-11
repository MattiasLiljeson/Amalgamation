#ifndef AGLTESTER_H
#define AGLTESTER_H

#include "AglReader.h"
#include "Utility.h"

class AGLTester
{
private:
	char* mPath;
	AglReader* mReader;
public:
	AGLTester(string pPath);
	~AGLTester();
	void PerformTest();
};

#endif