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
	/*int leftBtnState;*/
	Control* leftBtn;
	Control* xNegative;
	Control* xPositive;
	Control* yNegative;
	Control* yPositive;

	Cursor() // init all members to zero/NULL
	{
		//leftBtnState = InputHelper::KEY_STATE::KEY_UP;
		x = y = 0;
		xPositive = xNegative = yPositive = yNegative = NULL;
	}

	void init()
	{
		
	}

	void update()
	{
		// raw data for the mouse buttons are enums of type KEY_STATE
		//leftBtnState = leftBtn->getRawData();

		x -= xNegative->getStatus()*25000;
		y -= yNegative->getStatus()*25000;
		x += xPositive->getStatus()*25000;
		y += yPositive->getStatus()*25000;
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

