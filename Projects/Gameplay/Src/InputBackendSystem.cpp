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

	XInputFetcher* xInput = new XInputFetcher();
	//IMouseKeyboardFetcher* milf = new MessageLoopFetcher( false );
	HWND hWnd = m_graphicsBackend->getWindowRef();
	IMouseKeyboardFetcher* directInput = new DirectInputFetcher( m_hInstance, hWnd, true, true );
	m_inputManager = new InputManager( directInput, xInput );

	InputControlFactory factory;
	Control* tempControl = NULL;
	int tempControlIdx = -1;

	// Mouse, no factory method for all yet
	tempControl = factory.createMouseMovement( InputHelper::MOUSE_AXIS::X_POSITIVE );
	saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_MOUSE_MOVE,
		InputHelper::MOUSE_AXIS::X_POSITIVE, tempControl, "Mouse X positive" );

	tempControl = factory.createMouseMovement( InputHelper::MOUSE_AXIS::X_NEGATIVE );
	saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_MOUSE_MOVE,
		InputHelper::MOUSE_AXIS::X_NEGATIVE, tempControl, "Mouse X negative" );

	tempControl = factory.createMouseMovement( InputHelper::MOUSE_AXIS::Y_POSITIVE );
	saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_MOUSE_MOVE,
		InputHelper::MOUSE_AXIS::Y_POSITIVE, tempControl, "Mouse Y positive" );

	tempControl = factory.createMouseMovement( InputHelper::MOUSE_AXIS::Y_NEGATIVE );
	saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_MOUSE_MOVE,
		InputHelper::MOUSE_AXIS::Y_NEGATIVE, tempControl, "Mouse Y negative" );

	tempControl = factory.createMouseButton( InputHelper::MOUSE_BTN::M_LBTN );
	saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_MOUSE_BTN,
		InputHelper::MOUSE_BTN::M_LBTN, tempControl, "Mouse left btn" );

	tempControl = factory.createMouseButton( InputHelper::MOUSE_BTN::M_RBTN );
	saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_MOUSE_BTN,
		InputHelper::MOUSE_BTN::M_RBTN, tempControl, "Mouse right btn" );


	tempControl = factory.createKeyboardKey( InputHelper::KEY_SPACE );
	saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_KEYBOARD,
		InputHelper::KEYBOARD_KEY::KEY_SPACE, tempControl, "Space" );

	tempControl = factory.createKeyboardKey( InputHelper::KEY_LCTRL );
	saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_KEYBOARD,
		InputHelper::KEYBOARD_KEY::KEY_LCTRL, tempControl, "LCtrl" );

	// All analogs: stick/triggers on gamepad
	vector<pair<string, Control*> > analogs = factory.create360controllerAnalogAll();
	for( int i=0, size = (int)analogs.size(); i<size; i++ )
	{
		saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_XINPUT_ANALOG,
			(InputHelper::XBOX360_CONTROLLER_ANALOG)(InputHelper::THUMB_LX_POSITIVE + i),
			analogs[i].second, analogs[i].first );
	}

	// All digital: buttons/dpad on gamepad
	vector<pair<string, Control*> > digitals = factory.create360controllerDigitalAll();
	for( int i=0, size = (int)digitals.size(); i<size; i++ )
	{
		saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_XINPUT_DIGITAL,
			(InputHelper::XBOX360_CONTROLLER_DIGITAL)(InputHelper::DPAD_UP + i),
			digitals[i].second, digitals[i].first );
	}

	// All chars on keyboard
	// letters
	vector<pair<string, Control*> > keysAtoZ = factory.createKeysAToZ();
	for( int i=0, size = (int)keysAtoZ.size(); i<size; i++ )
	{
		saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_KEYBOARD,
			(InputHelper::KEYBOARD_KEY)(InputHelper::KEYBOARD_KEY::KEY_A + i),
			keysAtoZ[i].second, keysAtoZ[i].first );
	}
	// numbers
	vector<pair<string, Control*> > keysZeroToNine = factory.createKeysZeroToNine();
	for( int i=0, size = (int)keysZeroToNine.size(); i<size; i++ )
	{
		saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_KEYBOARD,
			(InputHelper::KEYBOARD_KEY)(InputHelper::KEYBOARD_KEY::KEY_0 + i),
			keysZeroToNine[i].second, keysZeroToNine[i].first );
	}
	// numpad numbers
	vector<pair<string, Control*> > keysNumZeroToNumNine = factory.createKeysNumZeroToNumNine();
	for( int i=0, size = (int)keysNumZeroToNumNine.size(); i<size; i++ )
	{
		saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_KEYBOARD,
			(InputHelper::KEYBOARD_KEY)(InputHelper::KEYBOARD_KEY::KEY_NUM0 + i),
			keysNumZeroToNumNine[i].second, keysNumZeroToNumNine[i].first );
	}
	// special keys
	// space
	saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_KEYBOARD,
		(InputHelper::KEYBOARD_KEY)(InputHelper::KEYBOARD_KEY::KEY_SPACE),
		factory.createKeyboardKey(InputHelper::KEYBOARD_KEY::KEY_SPACE), "KEY_SPACE" );
	// backspace
	saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_KEYBOARD,
		(InputHelper::KEYBOARD_KEY)(InputHelper::KEYBOARD_KEY::KEY_BACKSPACE),
		factory.createKeyboardKey(InputHelper::KEYBOARD_KEY::KEY_BACKSPACE), "KEY_BACKSPACE" );
	// return
	saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_KEYBOARD,
		(InputHelper::KEYBOARD_KEY)(InputHelper::KEYBOARD_KEY::KEY_RETURN),
		factory.createKeyboardKey(InputHelper::KEYBOARD_KEY::KEY_RETURN), "KEY_RETURN" );
	// esc
	saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_KEYBOARD, 
		(InputHelper::KEYBOARD_KEY)(InputHelper::KEYBOARD_KEY::KEY_ESC),
		factory.createKeyboardKey(InputHelper::KEYBOARD_KEY::KEY_ESC), "KEY_ESCAPE" );
	// f4
	saveControl( InputHelper::INPUT_DEVICE_TYPE::IT_KEYBOARD, 
		(InputHelper::KEYBOARD_KEY)(InputHelper::KEYBOARD_KEY::KEY_F4),
		factory.createKeyboardKey(InputHelper::KEYBOARD_KEY::KEY_F4), "KEY_F4" );

	m_cursor = new Cursor();

	m_cursor->addControlSet(
		2500000.0, 2500000.0,
		getControlByEnum(InputHelper::MOUSE_AXIS::X_NEGATIVE),
		getControlByEnum(InputHelper::MOUSE_AXIS::X_POSITIVE),
		getControlByEnum(InputHelper::MOUSE_AXIS::Y_NEGATIVE),
		getControlByEnum(InputHelper::MOUSE_AXIS::Y_POSITIVE),
		getControlByEnum(InputHelper::MOUSE_BTN::M_LBTN),
		getControlByEnum(InputHelper::MOUSE_BTN::M_RBTN) );
	m_cursor->addControlSet(
		1000.0, 1000.0, 
		getControlByEnum(InputHelper::XBOX360_CONTROLLER_ANALOG::THUMB_LX_NEGATIVE),
		getControlByEnum(InputHelper::XBOX360_CONTROLLER_ANALOG::THUMB_LX_POSITIVE),
		getControlByEnum(InputHelper::XBOX360_CONTROLLER_ANALOG::THUMB_LY_POSITIVE),
		getControlByEnum(InputHelper::XBOX360_CONTROLLER_ANALOG::THUMB_LY_NEGATIVE),
		getControlByEnum(InputHelper::XBOX360_CONTROLLER_DIGITAL::BTN_A),
		getControlByEnum(InputHelper::XBOX360_CONTROLLER_DIGITAL::BTN_B) );

	m_cursor->setScreenSize((double)m_graphicsBackend->getGfxWrapper()->getWindowWidth(),
							(double)m_graphicsBackend->getGfxWrapper()->getWindowHeight());
}


void InputBackendSystem::process()
{
	float dt = m_world->getDelta();

	m_inputManager->update();

	int currentWidth = m_graphicsBackend->getGfxWrapper()->getWindowWidth();
	int currentHeight = m_graphicsBackend->getGfxWrapper()->getWindowHeight();

	if (currentHeight!=m_cursor->getCurrentScreenHeight() || 
		currentWidth!=m_cursor->getCurrentScreenWidth())
	{
		m_cursor->reset();
		m_cursor->setScreenSize(currentWidth,currentHeight);
	}
	Control* kb_control = getControlByEnum( InputHelper::KEYBOARD_KEY::KEY_ESC);
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

Control* InputBackendSystem::getControlByEnum( InputHelper::KEYBOARD_KEY p_key )
{
	return m_inputManager->getControl(m_keyIdxsbyEnum[p_key]);
}

Control* InputBackendSystem::getControlByEnum( InputHelper::MOUSE_BTN p_btn )
{
	return m_inputManager->getControl(m_mouseBtnIdxsbyEnum[p_btn]);
}

Control* InputBackendSystem::getControlByEnum( InputHelper::MOUSE_AXIS p_axis )
{
	return m_inputManager->getControl(m_mouseMoveIdxsbyEnum[p_axis]);
}

Control* InputBackendSystem::getControlByEnum( InputHelper::XBOX360_CONTROLLER_DIGITAL p_digital )
{
	return m_inputManager->getControl(m_xboxAnalogIdxsbyEnum[p_digital]);
}

Control* InputBackendSystem::getControlByEnum( InputHelper::XBOX360_CONTROLLER_ANALOG p_analog )
{
	return m_inputManager->getControl(m_xboxDigitalIdxsbyEnum[p_analog]);
}

void InputBackendSystem::saveControl( InputHelper::INPUT_DEVICE_TYPE p_deviceType, 
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

vector<int>* InputBackendSystem::vectorFromDeviceType( InputHelper::INPUT_DEVICE_TYPE p_deviceType )
{
	vector<int>* vec = NULL;
	switch( p_deviceType )
	{
	case InputHelper::INPUT_DEVICE_TYPE::IT_MOUSE_MOVE:
		vec = &m_mouseMoveIdxsbyEnum;
		break;

	case InputHelper::INPUT_DEVICE_TYPE::IT_MOUSE_BTN:
		vec = &m_mouseBtnIdxsbyEnum;
		break;

	case InputHelper::INPUT_DEVICE_TYPE::IT_KEYBOARD:
		vec = &m_keyIdxsbyEnum;
		break;

	case InputHelper::INPUT_DEVICE_TYPE::IT_XINPUT_DIGITAL:
		vec = &m_xboxAnalogIdxsbyEnum;
		break;

	case InputHelper::INPUT_DEVICE_TYPE::IT_XINPUT_ANALOG:
		vec = &m_xboxDigitalIdxsbyEnum;
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