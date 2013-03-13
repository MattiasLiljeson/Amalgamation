#include "libRocketBackendSystem.h"

#include "GraphicsBackendSystem.h"
#include "InputBackendSystem.h"
#include "LibRocketRenderInterface.h"
#include "LibRocketSystemInterface.h"
#include <AntTweakBarWrapper.h>
#include <Control.h>
#include <Cursor.h>
#include <Globals.h>
#include <GraphicsWrapper.h>
#include <DebugUtil.h>
#include <Rocket/Controls.h>
#include <EventInstancer.h>
#include "LibRocketInputHelper.h"
#include "ClientConnectToServerSystem.h"
#include "LibRocketEventManagerSystem.h"
#include "ClientStateSystem.h"

LibRocketBackendSystem::LibRocketBackendSystem( GraphicsBackendSystem* p_graphicsBackend
											   , InputBackendSystem* p_inputBackend )
	: EntitySystem( SystemType::LibRocketBackendSystem )
{
	m_graphicsBackend = p_graphicsBackend;
	m_inputBackend = p_inputBackend;
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

	// Debug help functionality
	if(m_inputBackend->getDeltaByEnum(InputHelper::KeyboardKeys::KeyboardKeys_0) > 0.5f){
		m_renderDebug = !m_renderDebug;
		Rocket::Debugger::SetVisible( m_renderDebug );
	}
	ClientStateSystem* stateSystem = static_cast<ClientStateSystem*>(m_world->
		getSystem(SystemType::ClientStateSystem));
	if(stateSystem->getCurrentState() == GameStates::MENU ||
		stateSystem->getCurrentState() == GameStates::LOBBY)
	{
		if(m_inputBackend->getDeltaByEnum(InputHelper::MouseButtons_RIGHT) > 0.0 ||
			m_inputBackend->getDeltaByEnum(InputHelper::Xbox360Digitals_SHOULDER_PRESS_L) > 0.0)
		{
			m_renderGUI = false;
		}
		else if(m_inputBackend->getDeltaByEnum(InputHelper::MouseButtons_RIGHT) < 0.0 ||
			m_inputBackend->getDeltaByEnum(InputHelper::Xbox360Digitals_SHOULDER_PRESS_L) < 0.0)
		{
			m_renderGUI = true;
		}
	}
	else if(stateSystem->getCurrentState() == GameStates::INGAME){
		if(m_inputBackend->getDeltaByEnum(InputHelper::KeyboardKeys::KeyboardKeys_9) > 0.5f ){
			m_renderGUI = !m_renderGUI;
		}
	}
	else
	{
		m_renderGUI = true;
	}
	
	processMouseMove();
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

void LibRocketBackendSystem::processMouseMove()
{
	GraphicsWrapper* gfx = m_graphicsBackend->getGfxWrapper();

	pair<int,int> mousePos = gfx->getScreenPixelPosFromNDC( (float)m_cursor->getX(),
		(float)m_cursor->getY());
	int mouseX = mousePos.first;
	int mouseY = mousePos.second;

	m_rocketContext->ProcessMouseMove( mouseX, mouseY, 0 );
	m_rocketContext->ShowMouseCursor(m_cursor->isVisible());
	if( m_cursor->getPrimaryState() == InputHelper::KeyStates_KEY_PRESSED )
	{
		m_rocketContext->ProcessMouseButtonDown( 0, 0 );
	}
	else if( m_cursor->getPrimaryState() == InputHelper::KeyStates_KEY_RELEASED )
	{
		m_rocketContext->ProcessMouseButtonUp( 0, 0 );
	}
}

void LibRocketBackendSystem::processKeyStates()
{
	for (int keyCode = 0; keyCode < InputHelper::KeyboardKeys_CNT; keyCode++)
	{
		InputHelper::KeyboardKeys kbk = (InputHelper::KeyboardKeys)keyCode;
		Control* control = m_inputBackend->getControlByEnum(kbk);
		if (LibRocketInputHelper::isKeyMapped(keyCode) && control->getDelta() != 0)
		{
			if (control->getStatus() > 0.5f)
			{
				/*DEBUGPRINT(((toString("Key ") +
							toString(keyCode) + 
							toString(" was pressed\n")).c_str()));*/
				m_rocketContext->ProcessKeyDown(
					LibRocketInputHelper::rocketKeyFromInputKey(keyCode), 0);

				char c = InputHelper::charFromKeyboardKey(kbk);
				if (c != InputHelper::NONPRINTABLE_CHAR)
				{	
					m_rocketContext->ProcessTextInput(c);
				}
			}
			else
			{
				/*DEBUGPRINT(((toString("Key ") +
							toString(keyCode) + 
							toString(" was released\n")).c_str()));*/
				m_rocketContext->ProcessKeyUp(
					LibRocketInputHelper::rocketKeyFromInputKey(keyCode), 0);
			}

		}
	}
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







