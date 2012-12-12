#include "SimpleTessShader.h"
#include "Camera.h"

SimpleTessShader::SimpleTessShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	mDevice			= pDevice;
	mDeviceContext	= pDeviceContext;

	D3DCompileFromFile(L"simpleHLSLTes.hlsl", 0, 0, "VShader", "vs_5_0", 0, 0, &mVSD.CompiledData, 0);
    D3DCompileFromFile(L"simpleHLSLTes.hlsl", 0, 0, "PShader", "ps_5_0", 0, 0, &mPSD.CompiledData, 0);
	D3DCompileFromFile(L"simpleHLSLTes.hlsl", 0, 0, "HShader", "hs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &mHSD.CompiledData, 0);
    D3DCompileFromFile(L"simpleHLSLTes.hlsl", 0, 0, "DShader", "ds_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &mDSD.CompiledData, 0);

	mDevice->CreateVertexShader(mVSD.CompiledData->GetBufferPointer(), mVSD.CompiledData->GetBufferSize(), NULL, &mVSD.Data);
	mDevice->CreatePixelShader(mPSD.CompiledData->GetBufferPointer(), mPSD.CompiledData->GetBufferSize(), NULL, &mPSD.Data);
	mDevice->CreateHullShader(mHSD.CompiledData->GetBufferPointer(), mHSD.CompiledData->GetBufferSize(), NULL, &mHSD.Data);
	mDevice->CreateDomainShader(mDSD.CompiledData->GetBufferPointer(), mDSD.CompiledData->GetBufferSize(), NULL, &mDSD.Data);

	D3D11_INPUT_ELEMENT_DESC PositionColor[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mDevice->CreateInputLayout(PositionColor, 2, mVSD.CompiledData->GetBufferPointer(), mVSD.CompiledData->GetBufferSize(), &mIL);

	D3D11_BUFFER_DESC globalBufferDesc;
	globalBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	globalBufferDesc.ByteWidth = sizeof(GlobalBuffer);
	globalBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	globalBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	globalBufferDesc.MiscFlags = 0;
	globalBufferDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&globalBufferDesc, NULL, &mGlobalBuffer);
}
SimpleTessShader::~SimpleTessShader()
{
	mVSD.CompiledData->Release();
	mVSD.Data->Release();
	mPSD.CompiledData->Release();
	mPSD.Data->Release();
	mIL->Release();
	mGlobalBuffer->Release();

}
VertexShaderData SimpleTessShader::GetVertexShader()
{
	return mVSD;
}
PixelShaderData	SimpleTessShader::GetPixelShader()
{
	return mPSD;
}
HullShaderData SimpleTessShader::GetHullShader()
{
	return mHSD;
}
DomainShaderData SimpleTessShader::GetDomainShader()
{
	return mDSD;
}
ID3D11InputLayout* SimpleTessShader::GetInputLayout()
{
	return mIL;
}
void SimpleTessShader::SetMatrixBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection)
{
	pWorld = pWorld.transpose();
	pView = pView.transpose();
	pProjection = pProjection.transpose();

	D3D11_MAPPED_SUBRESOURCE resource;
	GlobalBuffer* buffer;
	mDeviceContext->Map(mGlobalBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	buffer = (GlobalBuffer*)resource.pData;
	buffer->World = pWorld;
	buffer->View = pView;
	buffer->Projection = pProjection;
	buffer->CameraPosition = Camera::GetInstance()->GetPosition();

	mDeviceContext->Unmap(mGlobalBuffer, 0);

	unsigned int bufferNumber = 0;
	mDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &mGlobalBuffer);
	mDeviceContext->HSSetConstantBuffers(bufferNumber, 1, &mGlobalBuffer);
}