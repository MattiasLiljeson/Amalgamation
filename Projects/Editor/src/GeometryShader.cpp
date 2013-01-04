#include "GeometryShader.h"

GeometryShader::GeometryShader(ID3D11Device* pDevice, string p_path, string p_mainFunction)
{
	mDevice			= pDevice;

	ShaderData* dat = loadShaderBlob(p_path + ".blob");
	if (!dat)
	{
		ID3DBlob* compiled;
		ID3DBlob* error;
		D3DCompileFromFile(toWString(p_path).c_str(), 0, 0, p_mainFunction.c_str(), "gs_5_0", 0, 0, &compiled, &error);
		if (error)
		{
			char* msg = (char*)error->GetBufferPointer();
			int k = 0;
			k++;
		}
		mDevice->CreateGeometryShader(compiled->GetBufferPointer(), compiled->GetBufferSize(), NULL, &mData);
		saveShaderBlob(p_path + ".blob", compiled);
	}
	else
	{
		mDevice->CreateGeometryShader(dat->data, dat->size, NULL, &mData);
		delete dat;
	}
}
GeometryShader::~GeometryShader()
{
	mData->Release();
}