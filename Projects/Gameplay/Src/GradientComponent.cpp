#include "GradientComponent.h"

GradientComponent::GradientComponent( AglVector4 p_small, AglVector4 p_big ) 
	: Component(ComponentType::Gradient)
{
	m_color.layerOne = p_small;
	m_color.layerTwo = p_big;
}
