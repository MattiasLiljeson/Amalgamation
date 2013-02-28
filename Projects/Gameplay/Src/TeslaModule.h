#pragma once

#include <Component.h>
#include <ComponentFactory.h>

class TeslaModule: public Component
{
public:
	TeslaModule();
	~TeslaModule();
	void init( vector<ComponentData> p_initData );

public:
	static ComponentRegister<TeslaModule> s_reg;
};