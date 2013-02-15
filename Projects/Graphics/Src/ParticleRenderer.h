#pragma once

class ParticleShader;
class AglParticleSystem;
class AglVector4;
class ShaderFactory;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11DepthStencilState;
struct Texture;
struct RendererSceneInfo;

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

struct InstanceData;

class ParticleRenderer
{
public:
	ParticleRenderer(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext);
	~ParticleRenderer();
	void renderParticleSystem( AglParticleSystem* p_system, RendererSceneInfo* p_info, 
		const InstanceData& p_transform, Texture* p_texture, const AglVector4& p_uvRect);
private:
	void initShaders();
	void initDepthStencil();
	void initTexture();
	void renderParticles( AglParticleSystem* p_system, const int numOfParticles, Texture* p_texture );
private:
	ParticleShader*			m_shader;
	ShaderFactory*			m_shaderFactory;
	ID3D11DepthStencilState*m_depthStencil;
	ID3D11Buffer*			m_vertexBuffer;
	Texture*				m_stdTexture;

	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;
};