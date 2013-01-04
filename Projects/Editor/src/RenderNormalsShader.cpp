#include "RenderNormalsShader.h"
#include "AGLLoader.h"
#include "Camera.h"
#include "TextureManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "Scene.h"

RenderNormalsShader::RenderNormalsShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, VertexShader* pVS, VertexShader* pVS2, GeometryShader* pGS, PixelShader* pPS)
{
	mDevice			= pDevice;
	mDeviceContext	= pDeviceContext;

	mVS = pVS;
	mPS = pPS;
	mGS = pGS;
	mVS2 = pVS2;

	D3D11_INPUT_ELEMENT_DESC PositionNormalTexCoordWeights[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"AWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"AINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 1, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 1, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mDevice->CreateInputLayout(PositionNormalTexCoordWeights, 9, mVS->GetCompiledData(), mVS->GetCompiledSize(), &mIL);

	D3D11_INPUT_ELEMENT_DESC PositionNormalTexCoord[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mDevice->CreateInputLayout(PositionNormalTexCoord, 5, mVS2->GetCompiledData(), mVS2->GetCompiledSize(), &mIL2);


	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(SkeletonMeshShaderNormalBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&BufferDesc, NULL, &mBuffer);

	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(SkeletonMeshGSShaderNormalBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&BufferDesc, NULL, &mBuffer2);

	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(StandardShaderNormalBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	pDevice->CreateBuffer(&BufferDesc, NULL, &mBuffer3);
}
RenderNormalsShader::~RenderNormalsShader()
{
	mIL->Release();
	mBuffer->Release();

}
ID3D11VertexShader* RenderNormalsShader::GetVertexShader()
{
	return mVS->GetShader();
}
ID3D11VertexShader* RenderNormalsShader::GetVertexShader2()
{
	return mVS2->GetShader();
}
ID3D11GeometryShader* RenderNormalsShader::GetGeometryShader()
{
	return mGS->GetShader();
}
ID3D11PixelShader*	RenderNormalsShader::GetPixelShader()
{
	return mPS->GetShader();
}
ID3D11InputLayout* RenderNormalsShader::GetInputLayout()
{
	return mIL;
}
ID3D11InputLayout* RenderNormalsShader::GetInputLayout2()
{
	return mIL2;
}
void RenderNormalsShader::SetBuffer(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale, AglSkeleton* pSkeleton, float p_length)
{
	pWorld = AglMatrix::transpose(pWorld);
	pView = AglMatrix::transpose(pView);
	pProjection = AglMatrix::transpose(pProjection);

	//Copy the matrix palette buffer to the gpu
	D3D11_MAPPED_SUBRESOURCE resource;
	mDeviceContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	SkeletonMeshShaderNormalBuffer* buffer;
	buffer = (SkeletonMeshShaderNormalBuffer*)resource.pData;
	buffer->World = pWorld;
	buffer->View = pView;
	buffer->Projection = pProjection;
	buffer->Scale = pScale;

	unsigned int jointCount = pSkeleton->getHeader().jointCount;
	for (unsigned int i = 0; i < jointCount; i++)
	{
		AglMatrix am = pSkeleton->getInverseBindMatrix(i) * pSkeleton->getGlobalTransform(i);
		am = AglMatrix::transpose(am);
		buffer->Palette[i] = am;
	}

	mDeviceContext->Unmap(mBuffer, 0);

	unsigned int bufferNumber = 0;
	mDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &mBuffer);

	//Map GS buffer
	mDeviceContext->Map(mBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	SkeletonMeshGSShaderNormalBuffer* buffer2;
	buffer2 = (SkeletonMeshGSShaderNormalBuffer*)resource.pData;
	buffer2->View = pView;
	buffer2->Projection = pProjection;
	buffer2->length[0] = p_length;

	mDeviceContext->Unmap(mBuffer2, 0);
	mDeviceContext->GSSetConstantBuffers(bufferNumber, 1, &mBuffer2);
}
void RenderNormalsShader::SetBuffer2(AglMatrix pWorld, AglMatrix pView, AglMatrix pProjection, float pScale, float p_length)
{
	pWorld = AglMatrix::transpose(pWorld);
	pView = AglMatrix::transpose(pView);
	pProjection = AglMatrix::transpose(pProjection);

	//Copy the matrix palette buffer to the gpu
	D3D11_MAPPED_SUBRESOURCE resource;
	mDeviceContext->Map(mBuffer3, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	StandardShaderNormalBuffer* buffer;
	buffer = (StandardShaderNormalBuffer*)resource.pData;
	buffer->World = pWorld;
	buffer->View = pView;
	buffer->Projection = pProjection;
	buffer->Scale = pScale;

	mDeviceContext->Unmap(mBuffer3, 0);

	unsigned int bufferNumber = 0;
	mDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &mBuffer3);

	//Map GS buffer
	mDeviceContext->Map(mBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	SkeletonMeshGSShaderNormalBuffer* buffer2;
	buffer2 = (SkeletonMeshGSShaderNormalBuffer*)resource.pData;
	buffer2->View = pView;
	buffer2->Projection = pProjection;
	buffer2->length[0] = p_length;

	mDeviceContext->Unmap(mBuffer2, 0);
	mDeviceContext->GSSetConstantBuffers(bufferNumber, 1, &mBuffer2);
}