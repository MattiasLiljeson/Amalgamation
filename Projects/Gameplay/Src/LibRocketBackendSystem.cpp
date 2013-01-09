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


	int wndWidth = m_graphicsBackend->getGfxWrapper()->getWindowWidth();
	int wndHeight = m_graphicsBackend->getGfxWrapper()->getWindowdHeight();

	m_rocketContextName = "default_name"; // Change the name if using multiple contexts!

	m_rocketContext = Rocket::Core::CreateContext(
		Rocket::Core::String( m_rocketContextName.c_str() ),
		Rocket::Core::Vector2i( wndWidth, wndHeight) );

	Rocket::Debugger::Initialise( m_rocketContext );
	Rocket::Debugger::SetVisible( true );
	
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
	loadDocument( tmp.c_str() );

	//tmp = GUI_HUD_PATH + "main.rml";
	//loadDocument( tmp.c_str() );

	//tmp = GUI_HUD_PATH + "index.rml";
	//loadDocument( tmp.c_str() );

	//tmp = GUI_HUD_PATH + "window.rml";
	//loadDocument( tmp.c_str() );

	string cursorPath = GUI_CURSOR_PATH + "cursor.rml";
	if( m_rocketContext->LoadMouseCursor(cursorPath.c_str()) == NULL )
	{
		int breakHere = 0;
	}
}

bool LibRocketBackendSystem::loadFontFace( const char* p_fontPath )
{
	return Rocket::Core::FontDatabase::LoadFontFace( Rocket::Core::String(p_fontPath) );
}

int LibRocketBackendSystem::loadDocument( const char* p_filePath )
{
	int id = -1;
	Rocket::Core::ElementDocument* tmpDoc = NULL;
	tmpDoc = m_rocketContext->LoadDocument( Rocket::Core::String(p_filePath) );
	
	if( tmpDoc != NULL )
	{
		id = m_documents.size();
		m_documents.push_back( tmpDoc );
		tmpDoc->Show(); //HACK! Remove this when doing this from the outside
		tmpDoc->RemoveReference();
	}

	return id;
}

void LibRocketBackendSystem::updateElement( string p_element, string p_value )
{
	Rocket::Core::Element* element;
	element = m_documents[0]->GetElementById( p_element.c_str() );
	element->SetInnerRML( p_value.c_str() );
}

void LibRocketBackendSystem::process()
{
	int windowWidth = m_graphicsBackend->getGfxWrapper()->getWindowWidth();
	int windowHeight = m_graphicsBackend->getGfxWrapper()->getWindowdHeight();
	// NDC -> Screenspace
	int screenSpaceX = static_cast<int>((m_cursor->getX()+1) * windowWidth/2);
	int screenSpaceY = static_cast<int>((m_cursor->getY()+1) * windowHeight/2);

	m_rocketContext->ProcessMouseMove( screenSpaceX, screenSpaceY, 0 );
	if( m_cursor->getPrimaryState() == InputHelper::KEY_STATE::KEY_PRESSED )
	{
		m_rocketContext->ProcessMouseButtonDown( 0, 0 );
	}
	else if( m_cursor->getPrimaryState() == InputHelper::KEY_STATE::KEY_RELEASED )
	{
		m_rocketContext->ProcessMouseButtonUp( 0, 0 );
	}

	//Rocket::Core::Element* test = m_documents[0]->GetElementById("btn");
	//if (test != NULL)
	//	test->SetProperty("width", "800px");

	m_rocketContext->Update();
}

void LibRocketBackendSystem::render()
{
	m_rocketContext->Render();
}
