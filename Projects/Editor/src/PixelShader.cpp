#include "PixelShader.h"

PixelShader::PixelShader(ID3D11Device* pDevice, string p_path, string p_mainFunction)
{
	mDevice			= pDevice;

	ShaderData* dat = loadShaderBlob(p_path + ".blob");
	if (!dat)
	{
		ID3DBlob* compiled;
		D3DCompileFromFile(toWString(p_path).c_str(), 0, 0, p_mainFunction.c_str(), "ps_5_0", 0, 0, &compiled, 0);
		mDevice->CreatePixelShader(compiled->GetBufferPointer(), compiled->GetBufferSize(), NULL, &mData);
		saveShaderBlob(p_path + ".blob", compiled);
	}
	else
	{
		mDevice->CreatePixelShader(dat->data, dat->size, NULL, &mData);
		delete dat;
	}
}
PixelShader::~PixelShader()
{
	mData->Release();
}