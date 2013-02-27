#pragma once

#include <Component.h>
#include <ComponentFactory.h>

class GravityBombModule: public Component
{
public:
	GravityBombModule();
	~GravityBombModule();
	void init( vector<ComponentData> p_initData );

public:
	static ComponentRegister<GravityBombModule> s_reg;
};