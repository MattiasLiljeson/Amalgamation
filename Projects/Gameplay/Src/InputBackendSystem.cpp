#include "InputBackendSystem.h"

InputBackendSystem::InputBackendSystem(void)
	: EntitySystem( SystemType::InputBackendSystem )
{
}

InputBackendSystem::~InputBackendSystem(void)
{
	delete m_inputManager;
	m_inputManager = NULL;
}

void InputBackendSystem::initialize()
{
	XInputFetcher* xif = new XInputFetcher();
	MessageLoopFetcher* milf = new MessageLoopFetcher( false );
	m_inputManager = new InputManager( milf, xif );

	InputControlFactory factory;
	Control* tempControl = NULL;
	int tempControlIdx = -1;

	tempControl = factory.create360controllerDigital( InputHelper::BTN_B );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs["Gamepad B"] = tempControlIdx;

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

	tempControl = factory.createKeyboardKey( InputHelper::SPACE );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs["Space"] = tempControlIdx;

	tempControl = factory.createKeyboardKey( InputHelper::L );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs["Keyboard key L"] = tempControlIdx;
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
			double x = 0.0, y = 0.0;
			x += m_inputManager->getControl(m_controlIdxs["Mouse X positive"])->getStatus();
			x -= m_inputManager->getControl(m_controlIdxs["Mouse X negative"])->getStatus();
			y += m_inputManager->getControl(m_controlIdxs["Mouse Y positive"])->getStatus();
			y -= m_inputManager->getControl(m_controlIdxs["Mouse Y negative"])->getStatus();

			AglVector3 position = transform->getTranslation();
			double sensitivityMult = 1000.0;
			position.x -= x*sensitivityMult;
			position.y -= y*sensitivityMult;
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
