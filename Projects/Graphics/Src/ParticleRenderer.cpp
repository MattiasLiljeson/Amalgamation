#include "ParticleRenderer.h"
#include "ShaderFactory.h"
#include <d3d11.h>
#include <AglMatrix.h>
#include <AglParticleSystem.h>
#include "ParticleShader.h"
#include <AglStandardParticle.h>
#include "Texture.h"
#include "TextureParser.h"
#include "ParticleCBuffer.h"
#include <AglVector4.h>

ParticleRenderer::ParticleRenderer( ID3D11Device* p_device, 
								   ID3D11DeviceContext* p_deviceContext ){
	m_device = p_device;
	m_deviceContext = p_deviceContext;
	m_vertexBuffer = NULL;

	m_shaderFactory = new ShaderFactory(m_device,m_deviceContext,
		m_device->GetFeatureLevel());

	initShaders();
	initDepthStencil();
	initTexture();
}

ParticleRenderer::~ParticleRenderer(){
	SAFE_RELEASE(m_depthStencil);
	delete m_shaderFactory;
	delete m_shader;
	delete m_texture;
}

void ParticleRenderer::renderParticles(AglParticleSystem* p_system, 
									   RendererSceneInfo p_info){

	vector<AglStandardParticle> particles = p_system->getParticles();
	if (particles.size() > 0){
		SAFE_RELEASE(m_vertexBuffer);
		D3D11_BUFFER_DESC bD;
		ZeroMemory(&bD, sizeof(bD));
		bD.Usage			= D3D11_USAGE_DYNAMIC;
		bD.ByteWidth		= sizeof(AglStandardParticle)* particles.size();
		bD.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		bD.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA vD;
		vD.pSysMem			= &particles[0];
		vD.SysMemPitch		= 0;
		vD.SysMemSlicePitch	= 0;

		m_device->CreateBuffer(&bD, &vD, &m_vertexBuffer);
		
		Buffer<ParticleCBuffer>* data = m_shader->getPerSystemBuffer();
		data->accessBuffer.setParticleData(p_system->getHeader());

		beginRendering(p_system, particles.size());
	}
}

void ParticleRenderer::beginRendering(AglParticleSystem* p_system, 
									  const int numOfParticles){
	ID3D11DepthStencilState* old;
	UINT stencil;
	m_deviceContext->OMGetDepthStencilState(&old, &stencil);
	m_deviceContext->OMSetDepthStencilState(m_depthStencil, 1);

	m_shader->apply();

	UINT stride = sizeof(AglStandardParticle);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_deviceContext->PSSetShaderResources(0, 1, &m_texture->data);

	m_deviceContext->Draw(numOfParticles, 0);

	m_shader->unApply();
	m_deviceContext->OMSetDepthStencilState(old, 1);

}

void ParticleRenderer::initShaders(){
	m_shader = m_shaderFactory->createParticleShader(
		L"Shaders/ParticleEffects/particleShader.hlsl");
}

void ParticleRenderer::initDepthStencil(){
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

	m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencil);
}

void ParticleRenderer::initTexture()
{
	m_texture = new Texture(TextureParser::loadTexture(
		m_device,"Assets/Textures/Test/smoke.png"));
}