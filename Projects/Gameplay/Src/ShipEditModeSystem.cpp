#include "ShipEditModeSystem.h"

#include <TcpClient.h>
#include <AglQuaternion.h>
#include "InputBackendSystem.h"
#include "PhysicsSystem.h"
#include "Transform.h"
#include "ShipEditController.h"
#include "PacketType.h"
#include "NetworkType.h"
#include "NetworkSynced.h"
#include "Control.h"
#include "AntTweakBarWrapper.h"
#include "PhysicsBody.h"

#include "GameplayTags.h"

ShipEditControllerSystem::ShipEditControllerSystem( InputBackendSystem* p_inputBackend, 
									   PhysicsSystem* p_physicsSystem/*, 
									   TcpClient* p_client */ ) : 
					EntitySystem( SystemType::ShipEditControllerSystem, 3,
								  ComponentType::ComponentTypeIdx::ShipEditController,
								  ComponentType::ComponentTypeIdx::TAG_ShipEditMode,
								  ComponentType::ComponentTypeIdx::Transform)

{
	m_inputBackend = p_inputBackend;
	m_physics = p_physicsSystem;
	// m_client = p_client;
}

ShipEditControllerSystem::~ShipEditControllerSystem()
{

}

void ShipEditControllerSystem::initialize()
{
	m_shipInput.initGamePad(m_inputBackend);
	m_shipInput.initMouse(m_inputBackend);
	m_shipInput.initKeyboard(m_inputBackend);

	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::INPUT,
		"ControllerEpsilon", TwType::TW_TYPE_FLOAT, m_shipInput.getControllerEpsilonPointer(), 
		"min=0.0 max=1.0 step=0.05" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( AntTweakBarWrapper::INPUT,
		"Left Stick (raw)",	TwType::TW_TYPE_DIR3D, &m_shipInput.m_leftStickDir, "" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( AntTweakBarWrapper::INPUT,
		"Right Stick (raw)", TwType::TW_TYPE_DIR3D, &m_shipInput.m_rightStickDir, "" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( AntTweakBarWrapper::INPUT,
		"Left W/ Correction", TwType::TW_TYPE_DIR3D, &m_shipInput.m_leftStickDirWithCorrection, "" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( AntTweakBarWrapper::INPUT,
		"Right W/ Correction", TwType::TW_TYPE_DIR3D, &m_shipInput.m_rightStickDirWithCorrection, "" );

	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::INPUT,
		"MouseSensitivity", TwType::TW_TYPE_FLOAT, &m_shipInput.m_mouseSensitivity, "");
}

void ShipEditControllerSystem::processEntities( const vector<Entity*>& p_entities )
{
	if (p_entities.size()>0)
	{
		float dt = m_world->getDelta();
		// Fetch the status of the various input methods.
		ShipSystemsInputHelper::RawInputForces rawInput;
		m_shipInput.readAllTheInput(rawInput);
		// processed input
		ShipSystemsInputHelper::ResultingInputForces input(rawInput);

		m_shipInput.updateAntTweakBar(input, m_inputBackend);

		// Apply correction vectors to the analogue sticks.
		input.horizontalInput += static_cast<float>(m_shipInput.m_leftStickCorrection[0]);
		input.verticalInput += static_cast<float>(m_shipInput.m_leftStickCorrection[1]);

		// Apply a threshold value to eliminate some of the analogue stick noise.
		if( abs(input.horizontalInput) < m_shipInput.m_controllerEpsilon ) 
			input.horizontalInput = 0;

		if( abs(input.verticalInput) < m_shipInput.m_controllerEpsilon ) 
			input.verticalInput = 0;

		if( abs(input.strafeHorizontalInput) < m_shipInput.m_controllerEpsilon ) 
			input.strafeHorizontalInput = 0;

		if( abs(input.strafeVerticalInput) < m_shipInput.m_controllerEpsilon ) 
			input.strafeVerticalInput = 0;

		for(unsigned int i=0; i<p_entities.size(); i++ )
		{
			Entity* ship = p_entities[i];
			ShipEditController* controller = static_cast<ShipEditController*>(
				ship->getComponent( ComponentType::ComponentTypeIdx::ShipFlyController ) );


			// State switch handling
			if (input.stateSwitchInput>0.0f)
			{
				ship->removeComponent(ComponentType::TAG_ShipEditMode); // Disable this state...
				ship->addTag(ComponentType::TAG_ShipFlyMode, new ShipFlyMode_TAG());  // ...and switch to fly state.
				ship->applyComponentChanges();
			}
		}
	}
}
