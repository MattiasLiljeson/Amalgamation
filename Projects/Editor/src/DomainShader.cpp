#include "DomainShader.h"

DomainShader::DomainShader(ID3D11Device* pDevice, string p_path, string p_mainFunction)
{
	mDevice			= pDevice;

	ShaderData* dat = loadShaderBlob(p_path + ".blob");
	if (!dat)
	{
		ID3DBlob* compiled;
		ID3DBlob* error;
		if (D3DCompileFromFile(toWString(p_path).c_str(), 0, 0, p_mainFunction.c_str(), "ds_5_0", 0, 0, &compiled, &error) != S_OK)
		{
			char* msg = (char*)error->GetBufferPointer();
			return;
		}
		mDevice->CreateDomainShader(compiled->GetBufferPointer(), compiled->GetBufferSize(), NULL, &mData);
		saveShaderBlob(p_path + ".blob", compiled);
	}
	else
	{
		mDevice->CreateDomainShader(dat->data, dat->size, NULL, &mData);
		delete dat;
	}
}
DomainShader::~DomainShader()
{
	mData->Release();
}