#include "GlowAnimation.h"

GlowAnimation::GlowAnimation()
	: Component(ComponentType::GlowAnimation)
{
	color = startColor = AglVector4(1.0f, 1.0f, 1.0f, 0.0f);
	enabled = false;
	lifetime = maxLifetime = 0.0f;
}

GlowAnimation::GlowAnimation( AglVector4 p_startColor, bool p_enabled/*=false*/,
	float p_lifetime/*=0.0f*/ )
	: Component(ComponentType::GlowAnimation)
{
	color = startColor = p_startColor;
	enabled = p_enabled;
	lifetime = maxLifetime = p_lifetime;
}

void GlowAnimation::resetAnimation()
{
	enabled = true;
	lifetime = maxLifetime;
}

const float& GlowAnimation::getMaxLifetime() const
{
	return maxLifetime;
}

const AglVector4& GlowAnimation::getStartColor() const
{
	return startColor;
}