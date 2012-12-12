#include "FBXManager.h"

FBXManager* FBXManager::sInstance = NULL;

FBXManager::FBXManager()
{
	mManager = FbxManager::Create();
    FbxIOSettings* ios = FbxIOSettings::Create(mManager, IOSROOT);
    mManager->SetIOSettings(ios);
}
FBXManager::~FBXManager()
{
	mManager->Destroy();
}
FBXManager* FBXManager::GetInstance()
{
	if (!sInstance)
		sInstance = new FBXManager();
	return sInstance;
}
void FBXManager::Destroy()
{
	if (sInstance)
		delete sInstance;
}
FbxManager* FBXManager::GetManager()
{
	return mManager;
}