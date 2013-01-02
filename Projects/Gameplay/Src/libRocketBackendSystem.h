#pragma once


#include "Globals.h"
#include "GraphicsBackendSystem.h"
#include "InputBackendSystem.h"
#include "LibRocketRenderInterface.h"
#include "LibRocketSystemInterface.h"
#include <EntitySystem.h>

#include <string>
#include <vector>

#include "Control.h"
#include "AntTweakBarWrapper.h"

struct Cursor
{
	double x; // double used internally in input
	double y; // double used internally in input
	bool pressed;
	Control* click;
	Control* xNegative;
	Control* xPositive;
	Control* yNegative;
	Control* yPositive;

	Cursor() // init all members to zero/NULL
	{
		pressed = false;
		x = y = 0;
		xPositive = xNegative = yPositive = yNegative = NULL;
	}

	void init()
	{
		
	}

	void update()
	{
		if( click->getDelta() == -1 )
			pressed = true;
		else
			pressed = false;

		x += xNegative->getRawData();
		y += yNegative->getRawData();
	}
};

class LibRocketBackendSystem : public EntitySystem
{
public:
	LibRocketBackendSystem( GraphicsBackendSystem* p_graphicsBackend,
		InputBackendSystem* p_inputBackend );
	~LibRocketBackendSystem();

	void initialize();

	bool loadFontFace( const char* p_fontPath );
	int loadDocument( const char* p_filePath );

	void process();
	void render();

private:
	GraphicsBackendSystem* m_graphicsBackend;
	InputBackendSystem* m_inputBackend;

	Cursor m_cursor;

	std::string m_rocketContextName;
	Rocket::Core::Context* m_rocketContext;
	LibRocketRenderInterface* m_renderInterface;
	LibRocketSystemInterface* m_systemInterface;

	vector<Rocket::Core::ElementDocument*> m_documents;
};

