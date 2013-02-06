#include "InputBackendSystem.h"

#include "CameraInfo.h"
#include "GraphicsBackendSystem.h"
#include "Transform.h"
#include <Cursor.h>
#include <Control.h>
#include <DirectInputFetcher.h>
#include <IMouseKeyboardFetcher.h>
#include <InputControlFactory.h>
#include <InputManager.h>
#include <MessageLoopFetcher.h>
#include <Windows.h>
#include <XInputFetcher.h>
#include <map>
#include <string>
#include <GraphicsWrapper.h>

InputBackendSystem::InputBackendSystem( HINSTANCE p_hInstance,
									   GraphicsBackendSystem* p_graphicsBackend )
	: EntitySystem( SystemType::InputBackendSystem )
{
	m_hInstance = p_hInstance;
	m_graphicsBackend = p_graphicsBackend;

	m_cursor = NULL;
}

InputBackendSystem::~InputBackendSystem()
{
	delete m_inputManager;
	m_inputManager = NULL;

	delete m_cursor;
	m_cursor = NULL;
}

void InputBackendSystem::initialize()
{
	InputHelper::initialize();

	XInputFetcher* xInput = new XInputFetcher();
	HWND hWnd = m_graphicsBackend->getWindowRef();
	IMouseKeyboardFetcher* directInput = new DirectInputFetcher( m_hInstance, hWnd, true, true );
	m_inputManager = new InputManager( directInput, xInput );

	InputControlFactory factory;
	Control* tempControl = NULL;
	int tempControlIdx = -1;

	vector< pair< string, Control* > > mouseAxes = factory.createAllMouseAxes();
	for( unsigned int i=0; i<mouseAxes.size(); i++ )
	{
		saveControl( InputHelper::InputDeviceTypes_MOUSE_AXIS,
			InputHelper::MouseAxes_FIRST + i,
			mouseAxes[i].second, mouseAxes[i].first );
	}
	vector< pair< string, Control* > > mouseButtons = factory.createAllMouseButtons();
	for( unsigned int i=0; i<mouseButtons.size(); i++ )
	{
		saveControl( InputHelper::InputDeviceTypes_MOUSE_BUTTON,
			InputHelper::MouseButtons_FIRST + i,
			mouseButtons[i].second, mouseButtons[i].first );
	}

	saveControl( InputHelper::InputDeviceTypes_KEYBOARD,
		InputHelper::KeyboardKeys_SPACE,
		factory.createKeyboardKey( InputHelper::KeyboardKeys_SPACE ),
		"Space" );

	saveControl( InputHelper::InputDeviceTypes_KEYBOARD,
		InputHelper::KeyboardKeys_LCTRL,
		factory.createKeyboardKey( InputHelper::KeyboardKeys_LCTRL ),
		"LCtrl" );

	// All analogs: stick/triggers on gamepad
	vector<pair<string, Control*> > analogs = factory.createAll360Analogs();
	for( int i=0, size = (int)analogs.size(); i<size; i++ )
	{
		saveControl( InputHelper::InputDeviceTypes_XINPUT_ANALOG,
			(InputHelper::Xbox360Analogs)(InputHelper::Xbox360Analogs_THUMB_LX_POSITIVE + i),
			analogs[i].second, analogs[i].first );
	}

	// All digital: buttons/dpad on gamepad
	vector<pair<string, Control*> > digitals = factory.createAll360Digitals();
	for( int i=0, size = (int)digitals.size(); i<size; i++ )
	{
		saveControl( InputHelper::InputDeviceTypes_XINPUT_DIGITAL,
			(InputHelper::Xbox360Digitals)(InputHelper::Xbox360Digitals_DPAD_UP + i),
			digitals[i].second, digitals[i].first );
	}

	// All chars on keyboard
	// letters
	vector<pair<string, Control*> > keysAtoZ = factory.createAToZ();
	for( int i=0, size = (int)keysAtoZ.size(); i<size; i++ )
	{
		saveControl( InputHelper::InputDeviceTypes_KEYBOARD,
			(InputHelper::KeyboardKeys)(InputHelper::KeyboardKeys_A + i),
			keysAtoZ[i].second, keysAtoZ[i].first );
	}
	// numbers
	vector<pair<string, Control*> > keysZeroToNine = factory.create0To9();
	for( int i=0, size = (int)keysZeroToNine.size(); i<size; i++ )
	{
		saveControl( InputHelper::InputDeviceTypes_KEYBOARD,
			(InputHelper::KeyboardKeys)(InputHelper::KeyboardKeys_0 + i),
			keysZeroToNine[i].second, keysZeroToNine[i].first );
	}
	// numpad numbers
	vector<pair<string, Control*> > keysNumZeroToNumNine = factory.createNumpad0ToNumpad9();
	for( int i=0, size = (int)keysNumZeroToNumNine.size(); i<size; i++ )
	{
		saveControl( InputHelper::InputDeviceTypes_KEYBOARD,
			(InputHelper::KeyboardKeys)(InputHelper::KeyboardKeys_NUMPAD_0 + i),
			keysNumZeroToNumNine[i].second, keysNumZeroToNumNine[i].first );
	}
	// special keys
	// space
	saveControl( InputHelper::InputDeviceTypes_KEYBOARD,
		InputHelper::KeyboardKeys_SPACE,
		factory.createKeyboardKey(InputHelper::KeyboardKeys_SPACE),
		"KeyboardKeys_SPACE" );
	// backspace
	saveControl( InputHelper::InputDeviceTypes_KEYBOARD,
		InputHelper::KeyboardKeys_BACKSPACE,
		factory.createKeyboardKey(InputHelper::KeyboardKeys_BACKSPACE),
		"KEY_BACKSPACE" );
	// return
	saveControl( InputHelper::InputDeviceTypes_KEYBOARD,
		InputHelper::KeyboardKeys_RETURN,
		factory.createKeyboardKey(InputHelper::KeyboardKeys_RETURN),
		"KEY_RETURN" );
	// esc
	saveControl( InputHelper::InputDeviceTypes_KEYBOARD, 
		InputHelper::KeyboardKeys_ESC,
		factory.createKeyboardKey(InputHelper::KeyboardKeys_ESC),
		"KEY_ESCAPE" );
	// f4
	saveControl( InputHelper::InputDeviceTypes_KEYBOARD, 
		InputHelper::KeyboardKeys_F4,
		factory.createKeyboardKey(InputHelper::KeyboardKeys_F4),
		"KEY_F4" );
	// period
	saveControl( InputHelper::InputDeviceTypes_KEYBOARD,
		InputHelper::KeyboardKeys_PERIOD,
		factory.createKeyboardKey(InputHelper::KeyboardKeys_PERIOD),
		"KEY_PERIOD" );

	// Intitiate cursor
	m_cursor = new Cursor();
	m_cursor->addControlSet(
		40000.0, 40000.0, false,
		getControlByEnum( InputHelper::MouseAxes_X_NEGATIVE ),
		getControlByEnum( InputHelper::MouseAxes_X_POSITIVE ),
		getControlByEnum( InputHelper::MouseAxes_Y_NEGATIVE ),
		getControlByEnum( InputHelper::MouseAxes_Y_POSITIVE ),
		getControlByEnum( InputHelper::MouseButtons_LEFT ),
		getControlByEnum( InputHelper::MouseButtons_RIGHT ) );
	m_cursor->addControlSet(
		1000.0, 1000.0, true,
		getControlByEnum( InputHelper::Xbox360Analogs_THUMB_LX_NEGATIVE ),
		getControlByEnum( InputHelper::Xbox360Analogs_THUMB_LX_POSITIVE ),
		getControlByEnum( InputHelper::Xbox360Analogs_THUMB_LY_POSITIVE ),
		getControlByEnum( InputHelper::Xbox360Analogs_THUMB_LY_NEGATIVE ),
		getControlByEnum( InputHelper::Xbox360Digitals_BTN_A ),
		getControlByEnum( InputHelper::Xbox360Digitals_BTN_B ) );

	m_cursor->setScreenSize(m_graphicsBackend->getGfxWrapper()->getWindowWidth(),
							m_graphicsBackend->getGfxWrapper()->getWindowHeight());
}


void InputBackendSystem::process()
{
	float dt = m_world->getDelta();

	m_inputManager->update();

	int currentWidth	= m_graphicsBackend->getGfxWrapper()->getWindowWidth();
	int currentHeight	= m_graphicsBackend->getGfxWrapper()->getWindowHeight();

	int cursorWidth		= m_cursor->getCurrentScreenWidth();
	int cursorHeight	= m_cursor->getCurrentScreenHeight();

	if (currentHeight != cursorHeight || 
		currentWidth  != cursorWidth)
	{
		// Fetch old cursor pixel position and use it to then get a new ndc position,
		// based on the new window width and height.
		auto oldCursorPixelPos = m_graphicsBackend->getGfxWrapper()->
			getScreenPixelPosFromNDC(m_cursor->getX(), m_cursor->getY(), cursorWidth, cursorHeight);
		auto newNdcPos = m_graphicsBackend->getGfxWrapper()->
			getNDCPosFromScreenPixel(oldCursorPixelPos.first, oldCursorPixelPos.second);
		m_cursor->overridePosition(newNdcPos.first, newNdcPos.second);
		m_cursor->setScreenSize(currentWidth,currentHeight);
	}
	Control* kb_control = getControlByEnum( InputHelper::KeyboardKeys_ESC);
	if( kb_control && kb_control->getDelta()>0.5f )
		m_world->requestToShutDown();

	m_cursor->update(dt);
}

Cursor* InputBackendSystem::getCursor()
{
	return m_cursor;
}

Control* InputBackendSystem::getInputControl( const string& p_name )
{
	return m_inputManager->getControl(m_controlIdxs[p_name]);
}

Control* InputBackendSystem::getControlByEnum( InputHelper::KeyboardKeys p_key )
{
	return m_inputManager->getControl(m_keyIdxsbyEnum[p_key]);
}

Control* InputBackendSystem::getControlByEnum( InputHelper::MouseButtons p_btn )
{
	return m_inputManager->getControl(m_mouseBtnIdxsbyEnum[p_btn]);
}

Control* InputBackendSystem::getControlByEnum( InputHelper::MouseAxes p_axis )
{
	return m_inputManager->getControl(m_mouseMoveIdxsbyEnum[p_axis]);
}

Control* InputBackendSystem::getControlByEnum( InputHelper::Xbox360Digitals p_digital )
{
	return m_inputManager->getControl(m_xboxDigitalIdxsbyEnum[p_digital]);
}

Control* InputBackendSystem::getControlByEnum( InputHelper::Xbox360Analogs p_analog )
{
	return m_inputManager->getControl(m_xboxAnalogIdxsbyEnum[p_analog]);
}

void InputBackendSystem::setControllerVibration(float p_leftMotor, float p_rightMotor)
{
	XInputFetcher* xinput = m_inputManager->getXInputFetcher();
	if (xinput) xinput->vibrate(p_leftMotor,p_rightMotor);
}

void InputBackendSystem::saveControl( InputHelper::InputDeviceTypes p_deviceType, 
							int p_controlType, Control* p_control, const string& p_name )
{
	// Register Control 
	int controlIdx = m_inputManager->addControl( p_control );

	// Add idx to "name" map
	m_controlIdxs[p_name] = controlIdx;

	// Find right "enum" map and add idx to it
	vector<int>* vectorToAddto = vectorFromDeviceType( p_deviceType );
	expandIdxVectorIfNecessary( vectorToAddto, p_controlType );
	(*vectorToAddto)[p_controlType] = controlIdx;
}

vector<int>* InputBackendSystem::vectorFromDeviceType( InputHelper::InputDeviceTypes p_deviceType )
{
	vector<int>* vec = NULL;
	switch( p_deviceType )
	{
	case InputHelper::InputDeviceTypes_MOUSE_AXIS:
		vec = &m_mouseMoveIdxsbyEnum;
		break;

	case InputHelper::InputDeviceTypes_MOUSE_BUTTON:
		vec = &m_mouseBtnIdxsbyEnum;
		break;

	case InputHelper::InputDeviceTypes_KEYBOARD:
		vec = &m_keyIdxsbyEnum;
		break;

	case InputHelper::InputDeviceTypes_XINPUT_DIGITAL:
		vec = &m_xboxDigitalIdxsbyEnum;
		break;

	case InputHelper::InputDeviceTypes_XINPUT_ANALOG:
		vec = &m_xboxAnalogIdxsbyEnum;
		break;

	default:
		vec = NULL;
	}
	return vec;
}

void InputBackendSystem::expandIdxVectorIfNecessary( vector<int>* p_vec, int p_idx )
{
	if( p_vec->size() <= ( unsigned int ) p_idx )
	{
		p_vec->resize(p_idx+1);
	}
}