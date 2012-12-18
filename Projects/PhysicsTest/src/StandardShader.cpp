#include "StandardShader.h"

StandardShader::StandardShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	mDevice			= pDevice;
	mDeviceContext	= pDeviceContext;

	D3DCompileFromFile(L"PhysicsTest/standardHLSL.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, &mVSD.CompiledData, 0);
    D3DCompileFromFile(L"PhysicsTest/standardHLSL.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, &mPSD.CompiledData, 0);

	mDevice->CreateVertexShader(mVSD.CompiledData->GetBufferPointer(), mVSD.CompiledData->GetBufferSize(), NULL, &mVSD.Data);
	mDevice->CreatePixelShader(mPSD.CompiledData->GetBufferPointer(), mPSD.CompiledData->GetBufferSize(), NULL, &mPSD.Data);

	D3D11_INPUT_ELEMENT_DESC PositionNormalTexCoord[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mDevice->CreateInputLayout(PositionNormalTexCoord, 3, mVSD.CompiledData->GetBufferPointer(), mVSD.CompiledData->GetBufferSize(), &mIL);

	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(StandardShaderBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&BufferDesc, NULL, &mBuffer);
}
StandardShader::~StandardShader()
{
	mVSD.CompiledData->Release();
	mVSD.Data->Release();
	mPSD.CompiledData->Release();
	mPSD.Data->Release();
	mIL->Release();
	mBuffer->Release();

}
VertexShaderData StandardShader::GetVertexShader()
{
	return mVSD;
}
PixelShaderData	StandardShader::GetPixelShader()
{
	return mPSD;
}
ID3D11InputLayout* StandardShader::GetInputLayout()
{
	return mIL;
}
void StandardShader::SetBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale)
{
	pWorld = pWorld.transpose();
	pView = pView.transpose();
	pProjection = pProjection.transpose();

	D3D11_MAPPED_SUBRESOURCE resource;
	mDeviceContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	StandardShaderBuffer* buffer;
	buffer = (StandardShaderBuffer*)resource.pData;
	buffer->World = pWorld;
	buffer->View = pView;
	buffer->Projection = pProjection;
	buffer->Scale = pScale;

	mDeviceContext->Unmap(mBuffer, 0);

	unsigned int bufferNumber = 0;
	mDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &mBuffer);
}