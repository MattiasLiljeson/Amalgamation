#pragma once

#include <EntitySystem.h>
#include <string>
#include <vector>
#include <Rocket/Core.h>
#include <Rocket/Debugger.h>

class AntTweakBarWrapper;
class Control;
class Cursor;
class GraphicsBackendSystem;
class InputBackendSystem;
class LibRocketRenderInterface;
class LibRocketSystemInterface;

class LibRocketBackendSystem : public EntitySystem
{
public:
	LibRocketBackendSystem( GraphicsBackendSystem* p_graphicsBackend,
		InputBackendSystem* p_inputBackend );
	~LibRocketBackendSystem();

	void initialize();

	bool loadFontFace( const char* p_fontPath );
	int loadDocument( const char* p_filePath );

	void updateElement( string p_element, string p_value );

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

