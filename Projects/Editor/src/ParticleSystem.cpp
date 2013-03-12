#include "ParticleSystem.h"
#include "Globals.h"
#include "SphereMesh.h"
#include "ShaderManager.h"
#include "ParticleShader.h"
#include "Camera.h"
#include "ParticleStates.h"

ParticleSystem::ParticleSystem(AglParticleSystem* pSystem, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	mSystem = pSystem;
	mDevice = pDevice;
	mDeviceContext = pDeviceContext;
	mVB = NULL;

	D3D11_BLEND_DESC blendStateDesc; 
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
	blendStateDesc.AlphaToCoverageEnable = FALSE;
	blendStateDesc.IndependentBlendEnable = FALSE;        
	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	pDevice->CreateBlendState(&blendStateDesc, &mBlendState);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
}

ParticleSystem::~ParticleSystem()
{
	if (mVB)
		mVB->Release();
}
void ParticleSystem::Draw(float p_scale)
{
	m_scale = p_scale;
	AglMatrix psWorld = AglMatrix::createScaleMatrix(AglVector3(p_scale, p_scale, p_scale))
							*AglMatrix::createTranslationMatrix(mSystem->getHeader().spawnPoint);

	vector<AglStandardParticle> particles = mSystem->getParticles();
	//Update vertex buffer
	if (particles.size() > 0)
	{
		if (mVB)
			mVB->Release();
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(AglStandardParticle) * particles.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = &particles[0];
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		mDevice->CreateBuffer(&bd, &vertexData, &mVB);
	}

	if (mVB)
	{
		ID3D11DepthStencilState* old;
		UINT stencil;
		mDeviceContext->OMGetDepthStencilState(&old, &stencil);

		if (mSystem->getHeader().rasterizerMode == AglParticleSystemHeader::AglRasterizerMode_ALWAYS_ON_TOP)
			mDeviceContext->OMSetDepthStencilState(ParticleStates::CullNone, 1);
		else
			mDeviceContext->OMSetDepthStencilState(ParticleStates::CullZ, 1);

		if (mSystem->getHeader().blendMode == AglParticleSystemHeader::AglBlendMode_ADDITIVE)
			mDeviceContext->OMSetBlendState(ParticleStates::AdditiveBlending, NULL, 0xFFFFFF);
		else if (mSystem->getHeader().blendMode == AglParticleSystemHeader::AglBlendMode_ALPHA)
			mDeviceContext->OMSetBlendState(ParticleStates::AlphaBlending, NULL, 0xFFFFFF);
		else
			mDeviceContext->OMSetBlendState(ParticleStates::MultiplyBlending, NULL, 0xFFFFFF);

		ParticleShader* ps = ShaderManager::GetInstance()->GetParticleShader();
		ps->SetBuffer(this);
		mDeviceContext->VSSetShader(ps->GetVertexShader(), 0, 0);
		mDeviceContext->PSSetShader(ps->GetPixelShader(), 0, 0);	
		mDeviceContext->HSSetShader(NULL, 0, 0);
		mDeviceContext->DSSetShader(NULL, 0, 0);
		mDeviceContext->GSSetShader(ps->GetGeometryShader(), 0, 0);
		mDeviceContext->IASetInputLayout(ps->GetInputLayout());

		UINT stride = sizeof(AglStandardParticle);
		UINT offset = 0;
		mDeviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		mDeviceContext->Draw(particles.size(), 0);

		mDeviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFF);
		mDeviceContext->OMSetDepthStencilState(old, 1);
	}
}

