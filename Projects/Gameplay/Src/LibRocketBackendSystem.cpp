#include "libRocketBackendSystem.h"

#include "ClientConnectToServerSystem.h"
#include "ClientStateSystem.h"
#include "GraphicsBackendSystem.h"
#include "InputActionsBackendSystem.h"
#include "InputBackendSystem.h"
#include "LibRocketEventManagerSystem.h"
#include "LibRocketInputHelper.h"
#include "LibRocketRenderInterface.h"
#include "LibRocketSystemInterface.h"
#include <AntTweakBarWrapper.h>
#include <Control.h>
#include <Cursor.h>
#include <DebugUtil.h>
#include <EventInstancer.h>
#include <Globals.h>
#include <GraphicsWrapper.h>
#include <Rocket/Controls.h>

LibRocketBackendSystem::LibRocketBackendSystem( GraphicsBackendSystem* p_graphicsBackend,
	InputBackendSystem* p_inputBackend, InputActionsBackendSystem* p_actionBackend )
	: EntitySystem( SystemType::LibRocketBackendSystem )
{
	m_graphicsBackend = p_graphicsBackend;
	m_inputBackend = p_inputBackend;
	m_actionBackend = p_actionBackend;
	m_renderGUI = true;
	m_renderDebug = false;
}


LibRocketBackendSystem::~LibRocketBackendSystem()
{
	// The connect handler is also an EntitySystem, and thus owned by the SystemManager.
	// Therefor, it is unregistered manually.
	//m_eventManager->UnregisterEventHandler("join");

	//m_eventManager->Shutdown();
	//delete m_eventManager;

	for( unsigned int i=0; i<m_documents.size(); i++ )
	{
		m_rocketContext->UnloadDocument(m_documents[i]);
		m_documents[i]->RemoveReference();
	}

	m_rocketContext->RemoveReference(); //release context

	delete m_renderInterface;
	delete m_systemInterface;
}

void LibRocketBackendSystem::initialize()
{
	LibRocketInputHelper::initialize();

	m_renderInterface = new LibRocketRenderInterface( m_graphicsBackend->getGfxWrapper() );
	m_systemInterface = new LibRocketSystemInterface( m_world );

	Rocket::Core::SetSystemInterface(m_systemInterface);
	Rocket::Core::SetRenderInterface(m_renderInterface);
	Rocket::Core::Initialise();
	Rocket::Controls::Initialise();

	m_wndWidth = m_graphicsBackend->getGfxWrapper()->getWindowWidth();
	m_wndHeight = m_graphicsBackend->getGfxWrapper()->getWindowHeight();

	m_rocketContextName = "default_name"; // Change the name if using multiple contexts!

	m_rocketContext = Rocket::Core::CreateContext(
		Rocket::Core::String( m_rocketContextName.c_str() ),
		Rocket::Core::Vector2i( m_wndWidth, m_wndHeight) );

	Rocket::Debugger::Initialise( m_rocketContext );
	Rocket::Debugger::SetVisible( m_renderDebug );
	
	m_cursor = m_inputBackend->getCursor();

	// Load fonts and documents
	// TODO: Should be done by assemblage

	vector<string> fonts;
	//fonts.push_back( "Delicious-Roman.otf" );
	//fonts.push_back( "Delicious-Bold.otf" );
	//fonts.push_back( "Delicious-Italic.otf" );
	//fonts.push_back( "Delicious-Roman.otf" );
	fonts.push_back( "Armorhide.ttf" );
	fonts.push_back( "digital-display.ttf" );
	fonts.push_back( "Geo-Regular.ttf" );
	fonts.push_back( "EHSMB.ttf" );
	fonts.push_back( "esper.ttf" );
	fonts.push_back( "young.ttf" );
	fonts.push_back( "Sierra Madre.ttf" );
	fonts.push_back( "AIRSTREA.ttf" );
	fonts.push_back( "ARCADE_I.ttf" );
	fonts.push_back( "ARCADE_N.ttf" );
	fonts.push_back( "ARCADE_R.ttf" );
	for( unsigned int i=0; i<fonts.size(); i++ )
	{
		string tmp = GUI_FONT_PATH + fonts[i]; 
		loadFontFace( tmp.c_str() );
	}

	//m_eventManager = static_cast<LibRocketEventManager*>(
	//	m_world->getSystem(SystemType::LibRocketEventManager));
	// Initialise event instancer and handlers.
	//EventInstancer* eventInstancer = new EventInstancer(m_eventManager);
	//Rocket::Core::Factory::RegisterEventListenerInstancer(eventInstancer);
	//eventInstancer->RemoveReference();

	//m_eventManager->Initialize(m_rocketContext);

	/*
	string tmp;
	tmp = GUI_HUD_PATH + "hud.rml";
	//tmp = GUI_HUD_PATH + "infoPanel.rml";
	int i = loadDocument( tmp.c_str() );
	m_documents[i]->Hide();
	*/

	string cursorPath = GUI_CURSOR_PATH + "cursor.rml";
	loadCursor(cursorPath.c_str());
}

void LibRocketBackendSystem::loadFontFace( const char* p_fontPath )
{
	if(!Rocket::Core::FontDatabase::LoadFontFace( Rocket::Core::String(p_fontPath) )){
		DEBUGWARNING(( 
				(std::string("Failed to load font face! Path: ") +
				toString(p_fontPath)).c_str()
			));
	}
}

int LibRocketBackendSystem::loadDocument(const char* p_path, 
													  const char* p_windowName)
{
	int docId = loadDocumentP(
		(
		toString(p_path) + toString(p_windowName) +	toString(".rml")
		).c_str(),p_windowName);

	return docId;
}

int LibRocketBackendSystem::loadDocumentP( const char* p_filePath, const char* p_windowName/*=NULL*/)
{
	int docId = -1;
	Rocket::Core::ElementDocument* tmpDoc = NULL;
	tmpDoc = m_rocketContext->LoadDocument( Rocket::Core::String(p_filePath) );

	if( tmpDoc != NULL )
	{
		docId = m_documents.size();
		m_documents.push_back( tmpDoc );
		
		// Set the element's title on the title; IDd 'title' in the RML.
		Rocket::Core::Element* title = tmpDoc->GetElementById("title");
		if (title != NULL)
			title->SetInnerRML(tmpDoc->GetTitle());

		Rocket::Core::String storedName;
		// If no windowName was specified, extract it using the full filePath.
		if (p_windowName == NULL)
		{
			Rocket::Core::StringList splitPath;
			Rocket::Core::StringUtilities::ExpandString(splitPath, p_filePath, '/');
			 // The unformatted window name, needs to get rid of the file extension.
			std::string name = splitPath[splitPath.size()-1].CString();
			int splitPos = name.find_last_of('.');
			if (splitPos != -1)
				name.erase(splitPos);

			storedName = name.c_str();
		}
		else
		{
			storedName = p_windowName;
		}
		// Set the element's id and map the resulting name.
		m_documents[docId]->SetId(storedName);
		m_docStringIdMap[storedName] = docId;

		//tmpDoc->RemoveReference();
	}
	else{
		DEBUGWARNING(( 
			(std::string("Failed to load LibRocket document! Path: ") +
			toString(p_filePath)).c_str() ));
	}
	return docId;
}

int LibRocketBackendSystem::getDocumentByName( const char* p_name ) const
{
	auto it = m_docStringIdMap.find(p_name);
	if (it != m_docStringIdMap.end())
		return it->second;
	else
		return -1;
}

void LibRocketBackendSystem::loadCursor( const char* p_cursorPath )
{
	if( m_rocketContext->LoadMouseCursor(p_cursorPath) == NULL ){
		DEBUGWARNING((
			(std::string("Failed to load LibRocket Cursor! Path: ") + 
			toString(p_cursorPath)).c_str()));
	}
	else{
		DEBUGPRINT(( (std::string("Loaded LibRocket Cursor document (from ")+
			toString(p_cursorPath) + ").\n").c_str() ));
	}
}

void LibRocketBackendSystem::updateElement(int p_docId, string p_element, string p_value )
{
	Rocket::Core::Element* element;
	element = m_documents[p_docId]->GetElementById( p_element.c_str() );
	if (element)
		element->SetInnerRML( p_value.c_str() );
}

void LibRocketBackendSystem::changeValue( int p_docId, string p_element, string p_value )
{
	Rocket::Core::Element* element;
	Rocket::Controls::ElementFormControl* formControl;
	element = m_documents[p_docId]->GetElementById( p_element.c_str() );
	if(element){
		formControl = static_cast<Rocket::Controls::ElementFormControl*>(element);
		formControl->SetValue(p_value.c_str());
	}

}

void LibRocketBackendSystem::showDocument( int p_docId, 
								int p_focusFlags/*= Rocket::Core::ElementDocument::FOCUS*/)
{
	m_documents[p_docId]->Show(p_focusFlags);
}

void LibRocketBackendSystem::hideDocument( int p_docId )
{
	m_documents[p_docId]->Hide();
}


void LibRocketBackendSystem::focusDocument( int p_docId )
{
	m_documents[p_docId]->Focus();
}

void LibRocketBackendSystem::process()
{
	GraphicsWrapper* gfx = m_graphicsBackend->getGfxWrapper();

	if (m_wndWidth!=gfx->getWindowWidth() || m_wndHeight!=gfx->getWindowHeight())
	{
		m_wndWidth = gfx->getWindowWidth();
		m_wndHeight = gfx->getWindowHeight();
		m_renderInterface->updateOnWindowResize();
		m_rocketContext->SetDimensions(Rocket::Core::Vector2i(m_wndWidth,m_wndHeight));
	}

	// Show libRocket visual debugger
	if(m_inputBackend->getDeltaByEnum(InputHelper::KeyboardKeys::KeyboardKeys_0) > 0.5f){
		m_renderDebug = !m_renderDebug;
		Rocket::Debugger::SetVisible( m_renderDebug );
	}
	ClientStateSystem* stateSystem = static_cast<ClientStateSystem*>(m_world->
		getSystem(SystemType::ClientStateSystem));

	// Rotate ship in the menu. Hide the menu so that the ship can be seen
	//GameStates state = stateSystem->getCurrentState();
	//if( state == GameStates::MENU || state == GameStates::LOBBY ) {
	//	double secondaryDelta = m_actionBackend->getDeltaByAction(
	//		InputActionsBackendSystem::Actions_CURSOR_SECONDARY );
	//	if( secondaryDelta > 0.5) {
	//		m_renderGUI = false;
	//	} else if( secondaryDelta < -0.5 ) {
	//		m_renderGUI = true;
	//	}
	//}

	//// DEBUG: Turn off GUI during gameplay
	//else if(stateSystem->getCurrentState() == GameStates::INGAME) {
	//	if(m_inputBackend->getDeltaByEnum(InputHelper::KeyboardKeys::KeyboardKeys_9) > 0.5f ){
	//		m_renderGUI = !m_renderGUI;
	//	}
	//}

	//Always shot GUI otherwise
	//else {
	//	m_renderGUI = true;
	//}
	
	processCursorMove();
	processKeyStates();

	m_rocketContext->Update();
}

void LibRocketBackendSystem::render()
{
	if(m_renderGUI)
	{
		m_rocketContext->Render();
	}
}

Rocket::Core::Context* LibRocketBackendSystem::getContext() const
{
	return m_rocketContext;
}

void LibRocketBackendSystem::processCursorMove()
{
	GraphicsWrapper* gfx = m_graphicsBackend->getGfxWrapper();

	pair<int,int> cursorPos = gfx->getScreenPixelPosFromNDC( (float)m_cursor->getX(),
		(float)m_cursor->getY());
	int cursorX = cursorPos.first;
	int cursorY = cursorPos.second;

	int keyModifiers = getKeyModifiers();

	m_rocketContext->ProcessMouseMove( cursorX, cursorY, keyModifiers );
	m_rocketContext->ShowMouseCursor( m_cursor->isVisible() );

	if( m_cursor->getPrimaryState() == InputHelper::KeyStates_KEY_PRESSED ) {
		m_rocketContext->ProcessMouseButtonDown( 0, keyModifiers );
	} else if( m_cursor->getPrimaryState() == InputHelper::KeyStates_KEY_RELEASED ) {
		m_rocketContext->ProcessMouseButtonUp( 0, keyModifiers );
	}

	if( m_cursor->getSecondaryState() == InputHelper::KeyStates_KEY_PRESSED ) {
		m_rocketContext->ProcessMouseButtonDown( 1, keyModifiers );
	} else if( m_cursor->getSecondaryState() == InputHelper::KeyStates_KEY_RELEASED ) {
		m_rocketContext->ProcessMouseButtonUp( 1, keyModifiers );
	}
}

void LibRocketBackendSystem::processKeyStates()
{
	int keyModifiers = getKeyModifiers();

	bool caps = false;
	double shiftPressness = 0.0;
	shiftPressness += m_inputBackend->getStatusByEnum( InputHelper::KeyboardKeys_LEFT_SHIFT );
	shiftPressness += m_inputBackend->getStatusByEnum( InputHelper::KeyboardKeys_RIGHT_SHIFT );
	if( shiftPressness > 0.5 ) {
		caps = true;
	}


	for( int keyCode=0; keyCode<InputHelper::KeyboardKeys_CNT; keyCode++ )
	{
		InputHelper::KeyboardKeys key = (InputHelper::KeyboardKeys)keyCode;
		Control* control = m_inputBackend->getControlByEnum(key);
		if (LibRocketInputHelper::isKeyMapped(keyCode) && control->getDelta() != 0) {
			if (control->getStatus() > 0.5f)
			{
				m_rocketContext->ProcessKeyDown(
					LibRocketInputHelper::rocketKeyFromInputKey(keyCode), keyModifiers);

				char c = InputHelper::charFromKeyboardKey(key, caps);
				if (c != InputHelper::NONPRINTABLE_CHAR)
				{	
					m_rocketContext->ProcessTextInput(c);
				}
			}
			else
			{
				m_rocketContext->ProcessKeyUp(
					LibRocketInputHelper::rocketKeyFromInputKey(keyCode), keyModifiers);
			}

		}
	}
}


int LibRocketBackendSystem::getKeyModifiers()
{
	int keyModifiers = 0;

	double ctrlPressness = 0.0;
	ctrlPressness += m_inputBackend->getStatusByEnum( InputHelper::KeyboardKeys_LEFT_CTRL );
	ctrlPressness += m_inputBackend->getStatusByEnum( InputHelper::KeyboardKeys_RIGHT_CTRL );
	if( ctrlPressness > 0.5 ) {
		keyModifiers |= Rocket::Core::Input::KM_CTRL;
	}

	double shiftPressness = 0.0;
	shiftPressness += m_inputBackend->getStatusByEnum( InputHelper::KeyboardKeys_LEFT_SHIFT );
	shiftPressness += m_inputBackend->getStatusByEnum( InputHelper::KeyboardKeys_RIGHT_SHIFT );
	if( shiftPressness > 0.5 ) {
		keyModifiers |= Rocket::Core::Input::KM_SHIFT;
	}

	double altPressness = 0.0;
	altPressness += m_inputBackend->getStatusByEnum( InputHelper::KeyboardKeys_LEFT_ALT );
	altPressness += m_inputBackend->getStatusByEnum( InputHelper::KeyboardKeys_RIGHT_ALT );
	if( ctrlPressness > 0.5 ) {
		keyModifiers |= Rocket::Core::Input::KM_ALT;
	}

	double winPressness = 0.0;
	winPressness += m_inputBackend->getStatusByEnum( InputHelper::KeyboardKeys_LEFT_WIN );
	winPressness += m_inputBackend->getStatusByEnum( InputHelper::KeyboardKeys_RIGHT_WIN );
	if( winPressness > 0.5 ) {
		keyModifiers |= Rocket::Core::Input::KM_META;
	}

	double capsPressness = m_inputBackend->getStatusByEnum( InputHelper::KeyboardKeys_CAPS_LOCK );
	if( capsPressness > 0.5 ) {
		keyModifiers |= Rocket::Core::Input::KM_CAPSLOCK;
	}
	
	double numPressness = m_inputBackend->getStatusByEnum( InputHelper::KeyboardKeys_NUM_LOCK );
	if( numPressness > 0.5 ) {
		keyModifiers |= Rocket::Core::Input::KM_NUMLOCK;
	}

	double scrollPressness = m_inputBackend->getStatusByEnum( InputHelper::KeyboardKeys_SCROLL_LOCK );
	if( scrollPressness > 0.5 ) {
		keyModifiers |= Rocket::Core::Input::KM_SCROLLLOCK;
	}

	return keyModifiers;
}


void LibRocketBackendSystem::showCursor()
{
	m_rocketContext->ShowMouseCursor(true);
}

void LibRocketBackendSystem::hideCursor()
{
	m_rocketContext->ShowMouseCursor(false);
}

bool LibRocketBackendSystem::isDocumentVisible( int p_docId ) const
{
	return m_documents[p_docId]->IsVisible();
}







