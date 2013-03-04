#pragma once

#include <EntitySystem.h>
#include <string>
#include <vector>
#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include <RenderInterface.h>
#include <map>

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
	void process();

	void render();

	void loadFontFace( const char* p_fontPath );
	int loadDocument(const char* p_path, const char* p_windowName);

	// Gets a document that matches the name (a.k.a body id). This function can only be
	// used if the document was loaded by name.
	int getDocumentByName(const char* p_id) const;

	void loadCursor( const char* p_cursorPath );
	void updateElement(int p_docId, string p_element, string p_value );

	void showDocument(int p_docId, int p_focusFlags = Rocket::Core::ElementDocument::FOCUS);
	void hideDocument(int p_docId);
	void focusDocument(int p_docId);

	void showCursor();
	void hideCursor();


	Rocket::Core::Context* getContext() const;
private:
	void processMouseMove();
	void processKeyStates();
	int loadDocumentP( const char* p_filePath, const char* p_windowName=NULL);

private:
	bool m_renderGUI;
	bool m_renderDebug;
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
	map<Rocket::Core::String, int> m_docStringIdMap;
};

