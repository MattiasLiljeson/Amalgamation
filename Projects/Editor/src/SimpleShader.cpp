#include "SimpleShader.h"

SimpleShader::SimpleShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	mDevice			= pDevice;
	mDeviceContext	= pDeviceContext;

	D3DCompileFromFile(L"simpleHLSL.hlsl", 0, 0, "VShader", "vs_5_0", 0, 0, &mVSD.CompiledData, 0);
    D3DCompileFromFile(L"simpleHLSL.hlsl", 0, 0, "PShader", "ps_5_0", 0, 0, &mPSD.CompiledData, 0);

	mDevice->CreateVertexShader(mVSD.CompiledData->GetBufferPointer(), mVSD.CompiledData->GetBufferSize(), NULL, &mVSD.Data);
	mDevice->CreatePixelShader(mPSD.CompiledData->GetBufferPointer(), mPSD.CompiledData->GetBufferSize(), NULL, &mPSD.Data);

	D3D11_INPUT_ELEMENT_DESC PositionColor[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mDevice->CreateInputLayout(PositionColor, 2, mVSD.CompiledData->GetBufferPointer(), mVSD.CompiledData->GetBufferSize(), &mIL);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&matrixBufferDesc, NULL, &mMatrixBuffer);
}
SimpleShader::~SimpleShader()
{
	mVSD.CompiledData->Release();
	mVSD.Data->Release();
	mPSD.CompiledData->Release();
	mPSD.Data->Release();
	mIL->Release();
	mMatrixBuffer->Release();

}
VertexShaderData SimpleShader::GetVertexShader()
{
	return mVSD;
}
PixelShaderData	SimpleShader::GetPixelShader()
{
	return mPSD;
}
ID3D11InputLayout* SimpleShader::GetInputLayout()
{
	return mIL;
}
void SimpleShader::SetMatrixBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection)
{
	pWorld = AglMatrix::transpose(pWorld);
	pView = AglMatrix::transpose(pView);
	pProjection = AglMatrix::transpose(pProjection);

	D3D11_MAPPED_SUBRESOURCE resource;
	MatrixBuffer* buffer;
	mDeviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	buffer = (MatrixBuffer*)resource.pData;
	buffer->World = pWorld;
	buffer->View = pView;
	buffer->Projection = pProjection;

	mDeviceContext->Unmap(mMatrixBuffer, 0);

	unsigned int bufferNumber = 0;
	mDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);
}