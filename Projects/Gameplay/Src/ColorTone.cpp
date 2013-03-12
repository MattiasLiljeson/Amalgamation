#include "ColorTone.h"

ColorTone::ColorTone(): Component( ComponentType::ColorTone )
{
	toneEnabled=false;
}
ColorTone::ColorTone(AglVector4 p_color, bool p_enabled)
	: Component(ComponentType::ColorTone)
{
	color = p_color;
	toneEnabled = p_enabled;
}

ColorTone::~ColorTone()
{
}