#pragma once
#include <EntitySystem.h>
#include <vector>

using namespace std;


class LibRocketBackendSystem;

class HudSystem :	public EntitySystem
{
public:
	HudSystem( LibRocketBackendSystem* p_backend );
	~HudSystem();
	void initialize();
	void processEntities( const vector<Entity*>& p_entities );
	void setHUDVisebilty(bool p_setVisibility);
private:
	LibRocketBackendSystem* m_backend;
	bool m_hudVisible;
	bool m_currentState; //Checks if the current status has changed last frame
};