#include "ShaderManager.h"

ShaderManager* ShaderManager::sInstance = NULL;

ShaderManager::ShaderManager()
{
}
ShaderManager* ShaderManager::GetInstance()
{
	if (sInstance == NULL)
		sInstance = new ShaderManager();
	return sInstance;
}
void ShaderManager::Destroy()
{
	sInstance->Cleanup();
	delete sInstance;
}
bool ShaderManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	mSimpleShader = new SimpleShader(pDevice, pDeviceContext);
	mStandardShader = new StandardShader(pDevice, pDeviceContext);

	return true;
}
bool ShaderManager::Cleanup()
{
	delete mSimpleShader;
	delete mStandardShader;
	return true;
}
SimpleShader* ShaderManager::GetSimpleShader()
{
	return mSimpleShader;
}
SimpleTessShader* ShaderManager::GetSimpleTessShader()
{
	return mSimpleTessShader;
}
StandardShader* ShaderManager::GetStandardShader()
{
	return mStandardShader;
}
