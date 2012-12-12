#include "InputSystem.h"

InputSystem::InputSystem(void)
	: EntitySystem( SystemType::InputSystem, 1, ComponentType::Input )
{
}

InputSystem::~InputSystem(void)
{
	delete m_inputManager;
	m_inputManager = NULL;
}

void InputSystem::initialize()
{
	//XInputFetcher* xif = new XInputFetcher();
	MessageLoopFetcher* milf = new MessageLoopFetcher( false );
	m_inputManager = new InputManager( milf, /*xif*/ NULL );

	InputControlFactory factory;
	Control* tempControl = NULL;
	int tempControlIdx = -1;

	tempControl = factory.create360controllerDigital( InputHelper::BTN_B );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs.push_back(tempControlIdx);

	tempControl = factory.createMouseMovement( InputHelper::MOUSE_AXIS::X,
		InputHelper::SUB_AXIS::AXIS_POSITIVE );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs.push_back(tempControlIdx);

	tempControl = factory.createMouseMovement( InputHelper::MOUSE_AXIS::X,
		InputHelper::SUB_AXIS::AXIS_NEGATIVE );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs.push_back(tempControlIdx);

	tempControl = factory.createMouseMovement( InputHelper::MOUSE_AXIS::Y,
		InputHelper::SUB_AXIS::AXIS_POSITIVE );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs.push_back(tempControlIdx);

	tempControl = factory.createMouseMovement( InputHelper::MOUSE_AXIS::Y,
		InputHelper::SUB_AXIS::AXIS_NEGATIVE );
	tempControlIdx = m_inputManager->addControl( tempControl );
	m_controlIdxs.push_back(tempControlIdx);

	tempControl = factory.createKeyboardKey( InputHelper::L );
	tempControlIdx = m_inputManager->addControl( tempControl );

	m_controlIdxs.push_back(tempControlIdx);
}

void InputSystem::processEntities( const vector<Entity*>& p_entities )
{
	m_inputManager->update();

	double pressness = m_inputManager->getControl(m_controlIdxs[0])->getStatus();
	bool pressed = false; 
	if(pressness == 1.0f)
		pressed = true;

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
			double x = m_inputManager->getControl(m_controlIdxs[1])->getStatus();
			x -= m_inputManager->getControl(m_controlIdxs[2])->getStatus();
			double y = m_inputManager->getControl(m_controlIdxs[3])->getStatus();
			y -= m_inputManager->getControl(m_controlIdxs[4])->getStatus();

			AglVector3 position = transform->getTranslation();
			double sensitivityMult = 1000.0;
			position.x -= x*sensitivityMult;
			position.y -= y*sensitivityMult;
			transform->setTranslation( position );
		}

	}
	if( m_inputManager->getControl( m_controlIdxs[5] )->getDelta() == 1.0 )
	{
		// L pressed
		EntitySystem* connectionSystem = m_world->getSystem(
			SystemType::NetworkConnectoToServerSystem );

		connectionSystem->setEnabled( true );
	}
	else if( m_inputManager->getControl( m_controlIdxs[5] )->getDelta() == -1.0 )
	{
		// L released
	}
}
