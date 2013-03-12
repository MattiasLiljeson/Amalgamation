#include "TextureFactory.h"


TextureFactory::TextureFactory(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext,
							   ResourceManager<Texture>* p_textureManager )
{
	m_textureManager = p_textureManager;
	m_device = p_device;
	m_deviceContext = p_deviceContext;
}

unsigned int TextureFactory::createTexture( const string& p_name, 
											const string& p_path)
{
	int texFoundId = m_textureManager->getResourceId(p_name);
	unsigned int texResultId = 0;

	if (texFoundId==-1)  // if it does not exist, create new
	{
		Texture* tex;
		tex = new Texture(TextureParser::loadTexture(m_device, m_deviceContext,
			(p_path+p_name).c_str()) );
		texResultId = m_textureManager->addResource(p_name,tex);
	}
	else
	{
		texResultId = static_cast<unsigned int>(texFoundId);
	}
	return texResultId;
}

unsigned int TextureFactory::createTexture( const byte* p_source, int p_width,
											int p_height, int p_pitch, int p_bitLevel,
											TextureParser::TEXTURE_TYPE p_type )
{
	// Create texture name used by manager
	static int createdTextureCount = 0;
	createdTextureCount++;
	stringstream ss;
	ss<<"Created Texture "<< createdTextureCount;
	string textureName = ss.str();

	// Create texture
	Texture* tex = new Texture(
		TextureParser::createTexture( m_device, m_deviceContext, p_source, p_width, 
		p_height, p_pitch, p_bitLevel, p_type ) );
	int textureId = m_textureManager->addResource( textureName, tex );
	return textureId;
}