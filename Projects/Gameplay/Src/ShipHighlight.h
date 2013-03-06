#pragma once
#include <Component.h>
#include <AglVector4.h>


class ShipHighlight: public Component
{
public:
	ShipHighlight() : Component(ComponentType::ShipHighlight)
	{
		color = AglVector4(1, 1, 1, 0);
		active = true;
	}
	void setAdditiveTone(AglVector4 p_color)
	{
		color = p_color;
		color.x = -color.x;
	}
	void setTone(AglVector4 p_color)
	{
		color = p_color;
	}
	void setReplaceTone(AglVector4 p_color)
	{
		color = p_color;
		color.y = -color.y;
	}

public:
	AglVector4 color;
	bool active;
};