#include "InputBackendSystem.h"
InputBackendSystem::InputBackendSystem( HINSTANCE p_hInstance,
									   GraphicsBackendSystem* p_graphicsBackend )
	: EntitySystem( SystemType::InputBackendSystem )
{
	m_hInstance = p_hInstance;
	m_graphicsBackend = p_graphicsBackend;
}

InputBackendSystem::~InputBackendSystem()
{
	delete m_inputManager;
	m_inputManager = NULL;
}

void InputBackendSystem::initialize()
{

	XInputFetcher* xInput = new XInputFetcher();
	//IMouseKeyboardFetcher* milf = new MessageLoopFetcher( false );
	HWND hWnd = m_graphicsBackend->getWindowRef();
	IMouseKeyboardFetcher* directInput = new DirectInputFetcher( m_hInstance, hWnd, true, false );
	m_inputManager = new InputManager( directInput, xInput );

	InputControlFactory factory;
	Control* tempControl = NULL;
	int tempControlIdx = -1;

	// GAMEPAD
	tempControl = factory.create360controllerDigital( InputHelper::BTN_B );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs["Gamepad B"] = tempControlIdx;

	tempControl = factory.create360controllerAnalog( InputHelper::XBOX360_CONTROLLER_ANALOG::THUMB_LX,
		InputHelper::SUB_AXIS::AXIS_POSITIVE );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs["Gamepad LX positive"] = tempControlIdx;

	tempControl = factory.create360controllerAnalog( InputHelper::XBOX360_CONTROLLER_ANALOG::THUMB_LX,
		InputHelper::SUB_AXIS::AXIS_NEGATIVE );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs["Gamepad LX negative"] = tempControlIdx;

	tempControl = factory.create360controllerAnalog( InputHelper::XBOX360_CONTROLLER_ANALOG::THUMB_LY,
		InputHelper::SUB_AXIS::AXIS_POSITIVE );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs["Gamepad LY positive"] = tempControlIdx;

	tempControl = factory.create360controllerAnalog( InputHelper::XBOX360_CONTROLLER_ANALOG::THUMB_LY,
		InputHelper::SUB_AXIS::AXIS_NEGATIVE );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs["Gamepad LY negative"] = tempControlIdx;

	// MOUSE
	tempControl = factory.createMouseMovement( InputHelper::MOUSE_AXIS::X,
		InputHelper::SUB_AXIS::AXIS_POSITIVE );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs["Mouse X positive"] = tempControlIdx;

	tempControl = factory.createMouseMovement( InputHelper::MOUSE_AXIS::X,
		InputHelper::SUB_AXIS::AXIS_NEGATIVE );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs["Mouse X negative"] = tempControlIdx;

	tempControl = factory.createMouseMovement( InputHelper::MOUSE_AXIS::Y,
		InputHelper::SUB_AXIS::AXIS_POSITIVE );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs["Mouse Y positive"] = tempControlIdx;

	tempControl = factory.createMouseMovement( InputHelper::MOUSE_AXIS::Y,
		InputHelper::SUB_AXIS::AXIS_NEGATIVE );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs["Mouse Y negative"] = tempControlIdx;

	// KEYBOARD
	tempControl = factory.createKeyboardKey( InputHelper::KEY_SPACE );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs["Space"] = tempControlIdx;

	vector<Control*> keysAtoZ = factory.createKeysAToZ();
	for( int i=0, size = (int)keysAtoZ.size(); i<size; i++ )
	{
		stringstream ss;
		ss << "Keyboard key ";
		ss << (char)('A'+i);

		tempControlIdx = m_inputManager->addControl( keysAtoZ[i] );
		m_controlIdxs[ss.str()] = tempControlIdx;
	}

//	tempControl = factory.createKeyboardKey( InputHelper::KEY_L );
//	tempControlIdx = m_inputManager->addControl( tempControl );
//	m_controlIdxs["Keyboard key L"] = tempControlIdx;
//
//	tempControl = factory.createKeyboardKey( InputHelper::KEY_W );
//	tempControlIdx = m_inputManager->addControl( tempControl );
//	m_controlIdxs["Keyboard key W"] = tempControlIdx;
//
//	tempControl = factory.createKeyboardKey( InputHelper::KEY_S );
//	tempControlIdx = m_inputManager->addControl( tempControl );
//	m_controlIdxs["Keyboard key S"] = tempControlIdx;
}
/*
void InputBackendSystem::processEntities( const vector<Entity*>& p_entities )
{
	
	m_inputManager->update();
	
	if( p_entities.size() > 0 )
	{
		Input* input = static_cast<Input*>(
			p_entities[0]->getComponent( ComponentType::ComponentTypeIdx::Input ) );

		CameraInfo* cameraInfo = static_cast<CameraInfo*>(
			p_entities[0]->getComponent( ComponentType::ComponentTypeIdx::CameraInfo ) );

		Transform* transform = static_cast<Transform*>(
			p_entities[0]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		if( cameraInfo != NULL )
		{
			double x = 0.0, y = 0.0, z = 0.0;
			x += m_inputManager->getControl(m_controlIdxs["Mouse X positive"])->getStatus();
			x -= m_inputManager->getControl(m_controlIdxs["Mouse X negative"])->getStatus();
			y += m_inputManager->getControl(m_controlIdxs["Mouse Y positive"])->getStatus();
			y -= m_inputManager->getControl(m_controlIdxs["Mouse Y negative"])->getStatus();
			z += m_inputManager->getControl(m_controlIdxs["Keyboard key W"])->getStatus();
			z -= m_inputManager->getControl(m_controlIdxs["Keyboard key S"])->getStatus();

			AglVector3 position = transform->getTranslation();
			double sensitivityMult = 1000.0;
			position.x -= x*sensitivityMult;
			position.y -= y*sensitivityMult;
			position.z -= z; 
			transform->setTranslation( position );
		}

	}
	if( m_inputManager->getControl( m_controlIdxs["Keyboard key L"] )->getDelta() == 1.0 )
	{
		// L pressed
		EntitySystem* connectionSystem = m_world->getSystem(
			SystemType::NetworkConnectoToServerSystem );

		connectionSystem->setEnabled( true );
	}
	else if( m_inputManager->getControl( m_controlIdxs["Keyboard key L"] )->getDelta() == -1.0 )
	{
		// L released
	}
	
}
*/
void InputBackendSystem::process()
{
	m_inputManager->update();
}

Control* InputBackendSystem::getInputControl( const string& p_name )
{
	return m_inputManager->getControl(m_controlIdxs[p_name]);
}
