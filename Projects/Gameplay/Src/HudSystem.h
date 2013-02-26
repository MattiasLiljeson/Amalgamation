#pragma once
#include <EntitySystem.h>

class LibRocketBackendSystem;

class HudSystem :	public EntitySystem
{
public:
	enum HUD_TYPES
	{
		TIME, SCORE, MAPPING
	};
public:
	HudSystem( LibRocketBackendSystem* p_backend );
	~HudSystem();
	void initialize();
	void process();
	void setHUDVisebilty(bool p_setVisibility);
	void setHUDData(HUD_TYPES p_type, const char* p_value);
private:
	LibRocketBackendSystem* m_backend;
	bool m_hudVisible;
	bool m_currentState; //Checks if the current status has changed last frame
	int m_hudIndex;

};