#pragma once

#include "ComponentData.h"

#include <vector>

using namespace std;


class Component
{
public:
	Component();
	virtual ~Component() = 0;

	virtual void init( vector<ComponentData> p_initData ){}
private:
	static int m_counter;
	int m_id;
};

