#include "VertexShader.h"

VertexShader::VertexShader(ID3D11Device* pDevice, string p_path, string p_mainFunction)
{
	mDevice			= pDevice;

	mShaderData = loadShaderBlob(p_path + ".blob");
	if (!mShaderData)
	{
		ID3DBlob* compiled;
		D3DCompileFromFile(toWString(p_path).c_str(), 0, 0, p_mainFunction.c_str(), "vs_5_0", 0, 0, &compiled, 0);
		saveShaderBlob(p_path + ".blob", compiled);
		compiled->Release();
		mShaderData = loadShaderBlob(p_path + ".blob");
	}
	mDevice->CreateVertexShader(mShaderData->data, mShaderData->size, NULL, &mData);
	mLoadedFromFile = true;
}
VertexShader::~VertexShader()
{
	delete mShaderData;
	mData->Release();
}