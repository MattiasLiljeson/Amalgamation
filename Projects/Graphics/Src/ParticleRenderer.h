#pragma once

class ParticleShader;
class AglParticleSystem;
class ShaderFactory;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11BlendState;
struct ID3D11DepthStencilState;
struct Texture;

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
	void renderParticles(AglParticleSystem* p_system);
private:
	void initShaders();
	void initBlendState();
	void initDepthStencil();
	void initTexture();
	void beginRendering(AglParticleSystem* p_system, const int numOfParticles);
private:
	ParticleShader*			m_shader;
	ShaderFactory*			m_shaderFactory;
	ID3D11BlendState*		m_blendState;
	ID3D11DepthStencilState*m_depthStencil;
	ID3D11Buffer*			m_vertexBuffer;
	Texture*				m_texture;

	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;
};