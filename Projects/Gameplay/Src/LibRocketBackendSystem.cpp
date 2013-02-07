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

LibRocketBackendSystem::LibRocketBackendSystem( GraphicsBackendSystem* p_graphicsBackend
											   , InputBackendSystem* p_inputBackend )
	: EntitySystem( SystemType::LibRocketBackendSystem )
{
	m_graphicsBackend = p_graphicsBackend;
	m_inputBackend = p_inputBackend;
}


LibRocketBackendSystem::~LibRocketBackendSystem()
{
	for( unsigned int i=0; i<m_documents.size(); i++ )
	{
		m_documents[i]->GetContext()->UnloadDocument(m_documents[i]);
	}

	m_rocketContext->RemoveReference(); //release context

	delete m_renderInterface;
	delete m_systemInterface;
}

void LibRocketBackendSystem::initialize()
{
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

//	Rocket::Debugger::Initialise( m_rocketContext );
//	Rocket::Debugger::SetVisible( true );
	
	m_cursor = m_inputBackend->getCursor();

	// Load fonts and documents
	// TODO: Should be done by assemblage

	vector<string> fonts;
	fonts.push_back( "Delicious-Roman.otf" );
	fonts.push_back( "Delicious-Bold.otf" );
	fonts.push_back( "Delicious-Italic.otf" );
	fonts.push_back( "Delicious-Roman.otf" );
	for( unsigned int i=0; i<fonts.size(); i++ )
	{
		string tmp = GUI_FONT_PATH + fonts[i]; 
		loadFontFace( tmp.c_str() );
	}

	string tmp;
	tmp = GUI_HUD_PATH + "hud.rml";
	//tmp = GUI_HUD_PATH + "infoPanel.rml";
	loadDocument( tmp.c_str() );

	//tmp = GUI_HUD_PATH + "main.rml";
	//loadDocument( tmp.c_str() );

	//tmp = GUI_HUD_PATH + "index.rml";
	//loadDocument( tmp.c_str() );

	//tmp = GUI_HUD_PATH + "window.rml";
	//loadDocument( tmp.c_str() );

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

int LibRocketBackendSystem::loadDocument( const char* p_filePath, bool p_initiallyShown/*=true*/)
{
	int docId = -1;
	Rocket::Core::ElementDocument* tmpDoc = NULL;
	tmpDoc = m_rocketContext->LoadDocument( Rocket::Core::String(p_filePath) );
	
	if( tmpDoc != NULL )
	{
		docId = m_documents.size();
		m_documents.push_back( tmpDoc );
		if (p_initiallyShown)
			tmpDoc->Show();
		tmpDoc->RemoveReference();
	}
	else{
		DEBUGWARNING(( 
			(std::string("Failed to load LibRocket document! Path: ") +
			toString(p_filePath)).c_str() ));
	}
	return docId;
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


void LibRocketBackendSystem::showDocument( int p_docId )
{
	m_documents[p_docId]->Show();
}

void LibRocketBackendSystem::hideDocument( int p_docId )
{
	m_documents[p_docId]->Hide();
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

	m_rocketContext->Update();
}

void LibRocketBackendSystem::render()
{
	m_rocketContext->Render();
}
