#pragma once

#include <Component.h>
#include <ComponentFactory.h>

class EmpModule: public Component
{
public:
	EmpModule();
	~EmpModule();
	void init( vector<ComponentData> p_initData );

public:
	static ComponentRegister<EmpModule> s_reg;
};