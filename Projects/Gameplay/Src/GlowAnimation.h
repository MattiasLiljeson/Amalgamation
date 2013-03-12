#pragma once
#include <Component.h>
#include <AglVector4.h>
// =======================================================================================
// GlowAnimation
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Contains a glow color to be interpolated towards "the natural" color.
///        
/// # GlowAnimation
/// Detailed description...
/// Created on: 11-3-2013 
///---------------------------------------------------------------------------------------
class GlowAnimation: public Component
{
public:
	GlowAnimation();
	GlowAnimation(AglVector4 p_startColor, bool p_enabled=false, float p_lifetime=0.0f);
	void resetAnimation();
	const float& getMaxLifetime() const;
	const AglVector4& getStartColor() const;

public:
	AglVector4 color;
	float lifetime;
	bool enabled;
	
private:
	AglVector4 startColor;
	float maxLifetime;
};