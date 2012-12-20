#pragma once
class Component
{
public:
	Component();
	virtual ~Component() = 0;
private:
	static int m_counter;
	int m_id;
};

