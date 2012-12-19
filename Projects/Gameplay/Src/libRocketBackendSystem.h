#pragma once

#include <EntitySystem.h>

#include "Globals.h"
#include "LibRocketRenderInterface.h"
#include "LibRocketSystemInterface.h"
#include "GraphicsBackendSystem.h"

#include <string>
#include <vector>

class LibRocketBackendSystem : public EntitySystem
{
public:
	LibRocketBackendSystem( GraphicsBackendSystem* p_graphicsBackend );
	~LibRocketBackendSystem();

	void initialize();

	bool loadFontFace( const char* p_fontPath );
	int loadDocument( const char* p_filePath );

	void process();
	void render();

private:
	GraphicsBackendSystem* m_graphicsBackend;
	std::string m_contextName;
	Rocket::Core::Context* m_rocketContext;
	LibRocketRenderInterface* m_renderInterface;
	LibRocketSystemInterface* m_systemInterface;

	vector<Rocket::Core::ElementDocument*> m_documents;
};

