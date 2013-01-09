#pragma once
#include <Component.h>
#include <string>

using namespace std;

class HudElement : public Component
{
public:
	HudElement( string p_element );
	~HudElement();

	void setValue( string p_value );
	bool hasChanged();
	void setRead();
	string getElement();
	string getValue();

private:
	bool m_changed;
	string m_value;
	string m_element;
};

