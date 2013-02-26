#pragma once

#include <Component.h>
#include <ComponentFactory.h>

class FlareGunModule: public Component
{
public:
	FlareGunModule();
	~FlareGunModule();
	void init( vector<ComponentData> p_initData );

public:
	static ComponentRegister<FlareGunModule> s_reg;
};