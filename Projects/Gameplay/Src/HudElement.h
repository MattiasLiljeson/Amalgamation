#pragma once
#include <Component.h>
#include <ComponentFactory.h>
#include <string>

using namespace std;

class HudElement : public Component
{
public:
	HudElement();
	HudElement( string p_element );
	~HudElement();

	virtual void init( vector<ComponentData> p_initData );

	void setValue( string p_value );
	bool hasChanged();
	void setRead();
	string getElement();
	string getValue();

private:
	static ComponentRegister<HudElement> s_reg;

	bool m_changed;
	string m_value;
	string m_element;
};

