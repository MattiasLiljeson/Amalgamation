#include "AglGradient.h"

AglGradient::AglGradient(string p_name)
{
	m_name = p_name;
	m_gradientTextureNameIndex = -1;
	m_normalTextureNameIndex = -1;
	m_layers.push_back(new AglGradientMaterial());
}
AglGradient::~AglGradient()
{
	for (unsigned int i = 0; i < m_layers.size(); i++)
	{
		delete m_layers[i];
	}
}
vector<AglGradientMaterial*> AglGradient::getLayers()
{
	return m_layers;
}
void AglGradient::addLayer(AglGradientMaterial* p_layer)
{
	m_layers.push_back(p_layer);
}
int AglGradient::getGradientTextureNameIndex()
{
	return m_gradientTextureNameIndex;
}
void AglGradient::setGradientTextureNameIndex(int p_index)
{
	m_gradientTextureNameIndex = p_index;
}
int AglGradient::getNormalTextureNameIndex()
{
	return m_normalTextureNameIndex;
}
void AglGradient::setNormalTextureNameIndex(int p_index)
{
	m_normalTextureNameIndex = p_index;
}