#pragma once
#include <EntitySystem.h>
#include <AglVector3.h>
#include <AglVector2.h>

class LibRocketBackendSystem;
class TcpClient;

class HudSystem :	public EntitySystem
{
public:
	enum HUD_TYPES
	{
		TIME, SCORE, 
		MAPPING_LEFT, MAPPING_RIGHT, MAPPING_UP, MAPPING_DOWN, 
		MASS, BOOST,
		PLAYERNAME, SERVERNAME
	};
public:
	HudSystem( LibRocketBackendSystem* p_backend, TcpClient* p_client );
	~HudSystem();
	void initialize();
	void process();
	void setHUDVisibility(bool p_setVisibility);
	void setHUDData(HUD_TYPES p_type, const char* p_value);
	bool isInited();
	void clear();
private:
	Entity* createSprite(AglVector3 p_position, string p_texture, AglVector2 p_size);
	Entity* createConstructionSprite(AglVector3 p_position, string p_texture, AglVector2 p_size);
	void reinitSprite(Entity* p_sprite, AglVector3 p_position, AglVector2 p_size);

private:
	LibRocketBackendSystem* m_backend;
	bool m_hudVisible;
	bool m_currentState; //Checks if the current status has changed last frame
	int m_hudIndex;
	bool m_isInited;

	AglVector2 m_screenSize;

	//Sprites
	Entity* m_leftWing;
	Entity* m_rightWing;
	Entity* m_timerMonitor;
	Entity* m_constructionMode;

	TcpClient* m_client;
};