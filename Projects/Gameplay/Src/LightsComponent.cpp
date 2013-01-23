#include "LightsComponent.h"

#include <LightInstanceData.h>
#include <AglMatrix.h>

Light::Light()
{
	//scale = AglVector3::one();
	//rotation = AglQuaternion( 0.0f, 0.0f, 0.0f, 0.0f );
	//translation = AglVector3::zero();
	offset = AglMatrix::identityMatrix();
}


LightsComponent::LightsComponent()
{
	m_type = ComponentType::LightsComponent;
}

LightsComponent::~LightsComponent()
{
}

vector<Light>* LightsComponent::getLightsPtr()
{
	return &m_lights;
}

void LightsComponent::addLight( Light p_light  )
{
	m_lights.push_back( p_light );
}
