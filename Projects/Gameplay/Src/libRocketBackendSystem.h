#pragma once


#include "Globals.h"
#include "GraphicsBackendSystem.h"
#include "InputBackendSystem.h"
#include "LibRocketRenderInterface.h"
#include "LibRocketSystemInterface.h"
#include <EntitySystem.h>

#include <string>
#include <vector>

#include <Control.h>
#include <Cursor.h>
#include <AntTweakBarWrapper.h>

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

	Cursor* m_cursor;

	std::string m_rocketContextName;
	Rocket::Core::Context* m_rocketContext;
	LibRocketRenderInterface* m_renderInterface;
	LibRocketSystemInterface* m_systemInterface;

	vector<Rocket::Core::ElementDocument*> m_documents;
};

