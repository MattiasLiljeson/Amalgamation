#include "ParticleRenderer.h"
#include "ShaderFactory.h"
#include <d3d11.h>

ParticleRenderer::ParticleRenderer( ID3D11Device* p_device, 
								   ID3D11DeviceContext* p_deviceContext ){
	m_device = p_device;
	m_deviceContext = p_deviceContext;

	m_shaderFactory = new ShaderFactory(m_device,m_deviceContext,
		m_device->GetFeatureLevel());

	initShaders();
}

ParticleRenderer::~ParticleRenderer(){

}

void ParticleRenderer::renderParticles( AglParticleSystem* p_particleSystem ){

}

void ParticleRenderer::initShaders(){
	m_shader = m_shaderFactory->createParticleShader(
		L"Shaders/ParticleEffects/particleShader.hlsl");
}

void ParticleRenderer::initBlendState()
{

}

void ParticleRenderer::initDepthStencil()
{

}