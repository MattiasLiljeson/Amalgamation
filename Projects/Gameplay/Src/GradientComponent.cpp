#include "GradientComponent.h"

GradientComponent::GradientComponent( AglVector4 p_small, AglVector4 p_big ) 
	: Component(ComponentType::Gradient)
{
	m_color.playerSmall = p_small;
	m_color.playerBig = p_big;
}
