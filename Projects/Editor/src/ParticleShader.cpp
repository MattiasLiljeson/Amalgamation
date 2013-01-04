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
		{"AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mDevice->CreateInputLayout(PositionNormalTexCoord, 4, mVS->GetCompiledData(), mVS->GetCompiledSize(), &mIL);
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

