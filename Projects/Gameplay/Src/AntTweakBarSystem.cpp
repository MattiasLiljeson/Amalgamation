#include "AntTweakBarSystem.h"
#include "GraphicsBackendSystem.h"
#include "InputBackendSystem.h"
#include <GraphicsWrapper.h>
#include <Cursor.h>
#include <Control.h>


AntTweakBarSystem::AntTweakBarSystem( GraphicsBackendSystem* p_gfxBackend,
									 InputBackendSystem* p_inputBackend )
									 : EntitySystem( SystemType::AntTweakBarSystem )
{
	m_gfxBackend = p_gfxBackend;
	m_inputBackend = p_inputBackend;
}


AntTweakBarSystem::~AntTweakBarSystem()
{
}

void AntTweakBarSystem::process()
{
	feedInput();
}

void AntTweakBarSystem::feedInput()
{
	GraphicsWrapper* gfx = m_gfxBackend->getGfxWrapper();
	AntTweakBarWrapper* antTweakBar = AntTweakBarWrapper::getInstance();

	// mouse pos
	Cursor* cursor = m_inputBackend->getCursor();
	pair<int,int> mousePos = gfx->getScreenPixelPosFromNDC( (float)cursor->getX(),
		(float)cursor->getY() );
	int mouseX = mousePos.first;
	int mouseY = mousePos.second;
	antTweakBar->setMousePos(mouseX,mouseY);

	// mouse key
	double l_btn = cursor->getPrimaryDelta();
	double r_btn = cursor->getSecondaryDelta();
	if( l_btn<-0.5f || l_btn>0.5f) antTweakBar->setMouseBtn( (int)(l_btn+1.0)/2, TW_MOUSE_LEFT );
	if( r_btn<-0.5f || r_btn>0.5f) antTweakBar->setMouseBtn( (int)(r_btn+1.0)/2, TW_MOUSE_RIGHT );

	// keyboard	A-Z
	Control* kb_control=NULL;
	for ( int letterIdx=InputHelper::KeyboardKeys_A; letterIdx<=InputHelper::KeyboardKeys_Z; letterIdx++ )
	{
		kb_control = m_inputBackend->getControlByEnum( (InputHelper::KeyboardKeys)letterIdx );
		if ( kb_control && kb_control->getDelta() > 0.5f )
		{
			antTweakBar->setKeyPressed(letterIdx+'A',0);
		}
	}

	// keyboard	0-9
	kb_control=NULL;
	for ( int keyNumIdx=InputHelper::KeyboardKeys_0; keyNumIdx<=InputHelper::KeyboardKeys_9; keyNumIdx++ )
	{
		kb_control = m_inputBackend->getControlByEnum( (InputHelper::KeyboardKeys)keyNumIdx );
		if ( kb_control && kb_control->getDelta() > 0.5f )
		{
			antTweakBar->setKeyPressed( keyNumIdx-(int)InputHelper::KeyboardKeys_0+'0', 0 );
		}
	}

	// keyboard	numpad 0-9
	kb_control=NULL;
	for (int numPadIdx=InputHelper::KeyboardKeys_NUMPAD_0; numPadIdx<=InputHelper::KeyboardKeys_NUMPAD_9; numPadIdx++)
	{
		kb_control = m_inputBackend->getControlByEnum( (InputHelper::KeyboardKeys)numPadIdx );
		if ( kb_control && kb_control->getDelta() > 0.5f )
		{
			antTweakBar->setKeyPressed( numPadIdx-(int)InputHelper::KeyboardKeys_NUMPAD_0+'0', 0 );
		}
	}

	// space
	kb_control = m_inputBackend->getControlByEnum( InputHelper::KeyboardKeys_SPACE );
	if ( kb_control && kb_control->getDelta() > 0.5f )
	{
		antTweakBar->setKeyPressed( TW_KEY_SPACE, 0 ); // Space in ASCII = 32
	}

	// backspace
	kb_control = m_inputBackend->getControlByEnum( InputHelper::KeyboardKeys_BACKSPACE );
	if ( kb_control && kb_control->getDelta() > 0.5f )
	{
		antTweakBar->setKeyPressed( TW_KEY_BACKSPACE, 0 ); // Backspace in ASCII = 8
	}

	// return
	kb_control = m_inputBackend->getControlByEnum( InputHelper::KeyboardKeys_RETURN );
	if ( kb_control && kb_control->getDelta() > 0.5f )
	{
		antTweakBar->setKeyPressed( TW_KEY_RETURN, 0 ); // Return in ASCII = 13
	}

	// F4
	kb_control = m_inputBackend->getControlByEnum( InputHelper::KeyboardKeys_F4);
	if ( kb_control && kb_control->getDelta() > 0.5f )
	{
		antTweakBar->setKeyPressed( TW_KEY_F4, 0 );
	}

}

void AntTweakBarSystem::render()
{
	AntTweakBarWrapper::getInstance()->render();
}
