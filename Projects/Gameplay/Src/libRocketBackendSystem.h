#pragma once

#include <EntitySystem.h>
#include <RenderInterface.h>
#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include <map>
#include <string>
#include <vector>

class AntTweakBarWrapper;
class Control;
class Cursor;
class EventHandler;
class GraphicsBackendSystem;
class InputActionsBackendSystem;
class InputBackendSystem;
class LibRocketRenderInterface;
class LibRocketSystemInterface;

class LibRocketBackendSystem : public EntitySystem, public RenderInterface
{
public:
	LibRocketBackendSystem( GraphicsBackendSystem* p_graphicsBackend,
		InputBackendSystem* p_inputBackend, InputActionsBackendSystem* p_actionBackend  );
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
	void changeValue(int p_docId, string p_element, string p_value );

	void showDocument(int p_docId, int p_focusFlags = Rocket::Core::ElementDocument::FOCUS);
	void hideDocument(int p_docId);
	bool isDocumentVisible(int p_docId) const;
	void focusDocument(int p_docId);

	void showCursor();
	void hideCursor();


	Rocket::Core::Context* getContext() const;
private:
	void processCursorMove();
	void processKeyStates();
	int getKeyModifiers();
	int loadDocumentP( const char* p_filePath, const char* p_windowName=NULL);

private:
	bool m_renderGUI;
	bool m_renderDebug;
	int m_wndHeight;
	int m_wndWidth;

	GraphicsBackendSystem* m_graphicsBackend;
	InputBackendSystem* m_inputBackend;
	InputActionsBackendSystem* m_actionBackend;

	Cursor* m_cursor;

	std::string					m_rocketContextName;
	Rocket::Core::Context*		m_rocketContext;
	LibRocketRenderInterface*	m_renderInterface;
	LibRocketSystemInterface*	m_systemInterface;

	vector<Rocket::Core::ElementDocument*> m_documents;
	map<Rocket::Core::String, int> m_docStringIdMap;
};

