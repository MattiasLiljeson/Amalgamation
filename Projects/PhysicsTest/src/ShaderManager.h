#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "SimpleTessShader.h"
#include "StandardShader.h"

class ShaderManager
{
private:
	SimpleShader* mSimpleShader;
	SimpleTessShader* mSimpleTessShader;
	StandardShader*		mStandardShader;

	static ShaderManager* sInstance;
private:
	ShaderManager();
public:
	static ShaderManager* GetInstance();
	static void Destroy();

	bool	Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	bool	Cleanup();

	SimpleShader* GetSimpleShader();
	SimpleTessShader* GetSimpleTessShader();
	StandardShader* GetStandardShader();

};

#endif