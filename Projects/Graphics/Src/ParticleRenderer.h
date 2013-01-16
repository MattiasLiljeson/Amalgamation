#pragma once

class ParticleShader;
class AglParticleSystem;
class ShaderFactory;
class ID3D11Device;
class ID3D11DeviceContext;
class ID3D11BlendState;
class ID3D11DepthStencilState;

// =======================================================================================
//                                      ParticleRenderer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the low level rendering of particles.
///        
/// # ParticleRenderer
/// Detailed description.....
/// Created on: 16-1-2013 
///---------------------------------------------------------------------------------------

class ParticleRenderer
{
public:
	ParticleRenderer(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext);
	~ParticleRenderer();
	void renderParticles(AglParticleSystem* p_particleSystem);
private:
	void initShaders();
	void initBlendState();
	void initDepthStencil();
private:
	ParticleShader*			m_shader;
	ShaderFactory*			m_shaderFactory;
	ID3D11BlendState*		m_blendState;
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;
};