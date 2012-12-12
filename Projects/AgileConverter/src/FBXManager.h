#ifndef FBXMANAGER_H
#define FBXMANAGER_H

#include "Utility.h"

class FBXManager
{
private:
	static FBXManager* sInstance;

	FbxManager* mManager;
private:
	FBXManager();
	~FBXManager();
public:
	static FBXManager* GetInstance();
	static void Destroy();
	FbxManager* GetManager();

};

#endif