#pragma once

#include <Component.h>
#include <vector>

using namespace std;

struct DisableData
{
	int target;
	bool frequency;
	bool color;
	DisableData(int p_target, bool p_frequency, bool p_color)
	{
		target = p_target;
		frequency = p_frequency;
		color = p_color;
	}
};

class InitiallyDisable : public Component
{
public:
	InitiallyDisable(): Component(ComponentType::InitiallyDisable)
	{

	}
public:
	vector<DisableData> data;
};