#include "ParticleShader.h"
#include "Camera.h"
#include "TextureManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "Scene.h"

ParticleShader::ParticleShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, VertexShader* pVS, PixelShader* pPS, GeometryShader* pGS )
{
	mDevice			= pDevice;
	mDeviceContext	= pDeviceContext;

	mVS = pVS;
	mPS = pPS;
	mGS = pGS;

	D3D11_INPUT_ELEMENT_DESC PositionNormalTexCoord[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mDevice->CreateInputLayout(PositionNormalTexCoord, 4, mVS->GetCompiledData(), mVS->GetCompiledSize(), &mIL);

	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(ParticleBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	mDevice->CreateBuffer(&BufferDesc, NULL, &mBuffer);
}

ParticleShader::~ParticleShader()
{
	mIL->Release();
}

ID3D11VertexShader* ParticleShader::GetVertexShader()
{
	return mVS->GetShader();
}

ID3D11PixelShader* ParticleShader::GetPixelShader()
{
	return mPS->GetShader();
}

ID3D11GeometryShader* ParticleShader::GetGeometryShader()
{
	return mGS->GetShader();
}

ID3D11InputLayout* ParticleShader::GetInputLayout()
{
	return mIL;
}
void ParticleShader::SetBuffer()
{
	AglMatrix v = Camera::GetInstance()->GetViewMatrix();
	AglMatrix p = Camera::GetInstance()->GetProjectionMatrix();
	AglVector3 pos = Camera::GetInstance()->GetPosition();
	v = v.transpose();
	p = p.transpose();

	D3D11_MAPPED_SUBRESOURCE resource;
	ParticleBuffer* buffer;
	mDeviceContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	buffer = (ParticleBuffer*)resource.pData;
	buffer->View = v;
	buffer->Projection = p;
	buffer->EyePosition = AglVector4(pos.x, pos.y, pos.z, 1);

	mDeviceContext->Unmap(mBuffer, 0);

	unsigned int bufferNumber = 0;
	mDeviceContext->GSSetConstantBuffers(bufferNumber, 1, &mBuffer);
}

