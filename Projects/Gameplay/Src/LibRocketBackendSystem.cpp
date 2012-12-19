#include "libRocketBackendSystem.h"

LibRocketBackendSystem::LibRocketBackendSystem( GraphicsBackendSystem* p_graphicsBackend )
	: EntitySystem( SystemType::LibRocketBackendSystem )
{
	m_graphicsBackend = p_graphicsBackend;
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

	m_contextName = "default_name"; // Change the name if using multiple contexts!

	m_rocketContext = Rocket::Core::CreateContext(
		Rocket::Core::String( m_contextName.c_str() ),
		Rocket::Core::Vector2i( wndWidth, wndHeight) );

	// Load fonts and documents
	// TODO: Should be done by assemblage

	vector<string> fonts;
	fonts.push_back( "Delicious-Roman.otf" );
	fonts.push_back( "Delicious-Bold.otf" );
	fonts.push_back( "Delicious-Italic.otf" );
	fonts.push_back( "Delicious-Roman.otf" );
	for( int i=0; i<fonts.size(); i++ )
	{
		string tmp = FONTPATH + fonts[i]; 
		loadFontFace( tmp.c_str() );
	}

	string tmp = HUDPATH + "demo.rml";
	loadDocument( tmp.c_str() );
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

void LibRocketBackendSystem::process()
{
	m_rocketContext->Update();
}

void LibRocketBackendSystem::render()
{
	m_rocketContext->Render();
}
