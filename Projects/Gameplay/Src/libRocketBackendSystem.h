#pragma once

#include <EntitySystem.h>
#include <string>
#include <vector>
#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include <RenderInterface.h>

class AntTweakBarWrapper;
class Control;
class Cursor;
class EventHandler;
class GraphicsBackendSystem;
class InputBackendSystem;
class LibRocketRenderInterface;
class LibRocketSystemInterface;

class LibRocketBackendSystem : public EntitySystem, public RenderInterface
{
public:
	LibRocketBackendSystem( GraphicsBackendSystem* p_graphicsBackend,
		InputBackendSystem* p_inputBackend );
	~LibRocketBackendSystem();

	void initialize();

	void loadFontFace( const char* p_fontPath );
	int loadDocumentByName(const char* p_windowName, 
							bool p_initiallyShown=false, 
							bool p_modal=false);
	int loadDocument( const char* p_filePath, bool p_initiallyShown=true);

	void loadCursor( const char* p_cursorPath );
	void updateElement(int p_docId, string p_element, string p_value );

	void showDocument(int p_docId, int p_focusFlags = Rocket::Core::ElementDocument::FOCUS);
	void hideDocument(int p_docId);
	void focusDocument(int p_docId);

	void showCursor();
	void hideCursor();

	void process();
	void render();

	Rocket::Core::Context* getContext() const;
private:
	void processMouseMove();
	void processKeyStates();

private:
	int m_wndHeight;
	int m_wndWidth;

	GraphicsBackendSystem* m_graphicsBackend;
	InputBackendSystem* m_inputBackend;

	Cursor* m_cursor;

	std::string					m_rocketContextName;
	Rocket::Core::Context*		m_rocketContext;
	LibRocketRenderInterface*	m_renderInterface;
	LibRocketSystemInterface*	m_systemInterface;

	vector<Rocket::Core::ElementDocument*> m_documents;
};

