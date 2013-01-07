#include "ShipControllerSystem.h"

#include <TcpClient.h>
#include <AglQuaternion.h>
#include "InputBackendSystem.h"
#include "PhysicsSystem.h"
#include "Transform.h"
#include "ShipController.h"
#include "PacketType.h"
#include "NetworkType.h"
#include "NetworkSynced.h"
#include "Control.h"
#include "AntTweakBarWrapper.h"


ShipControllerSystem::ShipControllerSystem( InputBackendSystem* p_inputBackend,
										    PhysicsSystem* p_physicsSystem,
											TcpClient* p_client ) : 
					  EntitySystem( SystemType::ShipControllerSystem, 2,
									ComponentType::ComponentTypeIdx::ShipController,
									ComponentType::ComponentTypeIdx::Transform)
{
	m_inputBackend = p_inputBackend;
	m_physics = p_physicsSystem;
	m_client = p_client;

	m_mouseSensitivity = 2000;

	m_controllerEpsilon = 0.15f;
	m_leftStickDir[0] = 0;
	m_leftStickDir[1] = 0;
	m_leftStickDir[2] = 0;
	m_rightStickDir[0] = 0;
	m_rightStickDir[1] = 0;
	m_rightStickDir[2] = 0;
	m_leftStickDirWithCorrection[0] = 0;
	m_leftStickDirWithCorrection[1] = 0;
	m_leftStickDirWithCorrection[2] = 0;
	m_rightStickDirWithCorrection[0] = 0;
	m_rightStickDirWithCorrection[1] = 0;
	m_rightStickDirWithCorrection[2] = 0;
	
	m_leftStickCorrection[0] = 0.1;
	m_leftStickCorrection[1] = 0.1;
	m_rightStickCorrection[0] = 0;
	m_rightStickCorrection[1] = 0;
}

ShipControllerSystem::~ShipControllerSystem()
{
}


void ShipControllerSystem::initGamePad()
{
	m_gamepadHorizontalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::THUMB_LX_POSITIVE);
	m_gamepadHorizontalNegative	= m_inputBackend->getControlByEnum( 
		InputHelper::THUMB_LX_NEGATIVE);
	m_gamepadVerticalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::THUMB_LY_POSITIVE);
	m_gamepadVerticalNegative	= m_inputBackend->getControlByEnum( 
		InputHelper::THUMB_LY_NEGATIVE);

	m_gamepadRollRight	= m_inputBackend->getControlByEnum( InputHelper::SHOULDER_PRESS_R);
	m_gamepadRollLeft	= m_inputBackend->getControlByEnum( InputHelper::SHOULDER_PRESS_L);

	m_gamepadThrust	= m_inputBackend->getControlByEnum( InputHelper::TRIGGER_R);

	m_gamepadStrafeHorizontalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::THUMB_RX_POSITIVE);
	m_gamepadStrafeHorizontalNegative	= m_inputBackend->getControlByEnum( 
		InputHelper::THUMB_RX_NEGATIVE);
	m_gamepadStrafeVerticalPositive	= m_inputBackend->getControlByEnum(
		InputHelper::THUMB_RY_POSITIVE);
	m_gamepadStrafeVerticalNegative	= m_inputBackend->getControlByEnum(
		InputHelper::THUMB_RY_NEGATIVE);
}


void ShipControllerSystem::initMouse()
{
	m_mouseHorizontalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::MOUSE_AXIS::X_POSITIVE);
	m_mouseHorizontalNegative	= m_inputBackend->getControlByEnum(
		InputHelper::MOUSE_AXIS::X_NEGATIVE);
	m_mouseVerticalPositive		= m_inputBackend->getControlByEnum( 
		InputHelper::MOUSE_AXIS::Y_POSITIVE);
	m_mouseVerticalNegative		= m_inputBackend->getControlByEnum( 
		InputHelper::MOUSE_AXIS::Y_NEGATIVE);
}

void ShipControllerSystem::initKeyboard()
{
	m_keyboardRollRight = m_inputBackend->getControlByEnum(
		InputHelper::KEY_D);
	m_keyboardRollLeft = m_inputBackend->getControlByEnum(
		InputHelper::KEY_A);

	m_keyboardThrust = m_inputBackend->getControlByEnum(
		InputHelper::KEY_W);

	m_keyboardStrafeVerticalPos = m_inputBackend->getControlByEnum(
		InputHelper::KEY_O);
	m_keyboardStrafeVerticalNeg = m_inputBackend->getControlByEnum(
		InputHelper::KEY_L);
	m_keyboarStrafeHorizontalPos = m_inputBackend->getControlByEnum(
		InputHelper::KEY_E);
	m_keyboarStrafeHorizontalNeg = m_inputBackend->getControlByEnum(
		InputHelper::KEY_Q);
}

void ShipControllerSystem::initialize()
{
	initGamePad();
	initMouse();
	initKeyboard();

	AntTweakBarWrapper::getInstance()->addWriteVariable( "ControllerEpsilon",
		TwType::TW_TYPE_FLOAT, getControllerEpsilonPointer(), "min=0.0 max=1.0 step=0.05" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( "Left Stick (raw)",
		TwType::TW_TYPE_DIR3D, &m_leftStickDir, "" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( "Right Stick (raw)",
		TwType::TW_TYPE_DIR3D, &m_rightStickDir, "" );
	
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( "Left W/ Correction",
		TwType::TW_TYPE_DIR3D, &m_leftStickDirWithCorrection, "" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( "Right W/ Correction",
		TwType::TW_TYPE_DIR3D, &m_rightStickDirWithCorrection, "" );

	AntTweakBarWrapper::getInstance()->addWriteVariable( "MouseSensitivity",
		TwType::TW_TYPE_FLOAT, &m_mouseSensitivity, "");
}

void ShipControllerSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();
	// Fetch the status of the various input methods.
	ResultingInputForces input = readAllTheInput();
	
	updateAntTweakBar(input);

	// Apply correction vectors to the analogue sticks.
	input.horizontalInput += static_cast<float>(m_leftStickCorrection[0]);
	input.verticalInput += static_cast<float>(m_leftStickCorrection[1]);

	// Apply a threshold value to eliminate some of the analogue stick noise.
	if( abs(input.horizontalInput) < m_controllerEpsilon ) input.horizontalInput = 0;
	if( abs(input.verticalInput) < m_controllerEpsilon ) input.verticalInput = 0;

	if( abs(input.strafeHorizontalInput) < m_controllerEpsilon ) input.strafeHorizontalInput = 0;
	if( abs(input.strafeVerticalInput) < m_controllerEpsilon ) input.strafeVerticalInput = 0;

	for(unsigned int i=0; i<p_entities.size(); i++ )
	{
		ShipController* controller = static_cast<ShipController*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::ShipController ) );

		Transform* transform = static_cast<Transform*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		// Calc rotation from player input
		AglVector3 inputAngles(input.verticalInput,input.horizontalInput,input.rollInput);

		// Turning multiplier
		float  turnSpeed = controller->getTurningSpeed() * dt;
		// Thrust multiplier
		float  thrustPower = controller->getThrustPower() * dt;

		// Calc translation from player input
		AglVector3 thrustVec;
		thrustVec += transform->getMatrix().GetForward()	* input.thrustInput 
			* thrustPower;
		thrustVec += transform->getMatrix().GetRight()		* input.strafeHorizontalInput 
			* thrustPower;
		thrustVec += transform->getMatrix().GetUp()			* input.strafeVerticalInput 
			* thrustPower;

		// Calc rotation from player input
		AglVector3 angularVec=inputAngles*turnSpeed;
		AglQuaternion quat = transform->getRotation();
		quat.transformVector(angularVec);

		Packet thrustPacket;
		NetworkSynced* netSync = static_cast<NetworkSynced*>(p_entities[i]->getComponent(
			ComponentType::NetworkSynced));

		thrustPacket << (char)NetworkType::Ship << (char)PacketType::PlayerInput 
			<< thrustVec << angularVec << netSync->getNetworkIdentity();
		m_client->sendPacket( thrustPacket );
	}
}

float* ShipControllerSystem::getControllerEpsilonPointer()
{
	return &m_controllerEpsilon;
}

ResultingInputForces ShipControllerSystem::readAllTheInput()
{
	RawInputForces input;
	input.hPositive = m_gamepadHorizontalPositive->getStatus();
	input.hPositive += m_mouseHorizontalPositive->getStatus()*m_mouseSensitivity;

	input.hNegative = m_gamepadHorizontalNegative->getStatus();
	input.hNegative += m_mouseHorizontalNegative->getStatus()*m_mouseSensitivity;

	input.vPositive = m_gamepadVerticalPositive->getStatus();
	input.vPositive += m_mouseVerticalPositive->getStatus()*m_mouseSensitivity;

	input.vNegative = m_gamepadVerticalNegative->getStatus();
	input.vNegative += m_mouseVerticalNegative->getStatus()*m_mouseSensitivity;

	input.shPositive = m_gamepadStrafeHorizontalPositive->getStatus();
	input.shPositive += m_keyboarStrafeHorizontalPos->getStatus();

	input.shNegative = m_gamepadStrafeHorizontalNegative->getStatus();
	input.shNegative += m_keyboarStrafeHorizontalNeg->getStatus();

	input.svPositive = m_gamepadStrafeVerticalPositive->getStatus();
	input.svPositive += m_keyboardStrafeVerticalPos->getStatus();

	input.svNegative = m_gamepadStrafeVerticalNegative->getStatus();
	input.svNegative += m_keyboardStrafeVerticalNeg->getStatus();

	input.rRight = m_gamepadRollRight->getStatus();
	input.rRight += m_keyboardRollRight->getStatus();

	input.rLeft = m_gamepadRollLeft->getStatus();
	input.rLeft += m_keyboardRollLeft->getStatus();

	input.thrust =  m_gamepadThrust->getStatus();
	input.thrust += m_keyboardThrust->getStatus();


	ResultingInputForces resultingInput(input);
	return resultingInput;
}

void ShipControllerSystem::updateAntTweakBar(const ResultingInputForces& p_input)
{
	// Calibrate the Gamepad's analogue sticks when pressing the C key.
	if( m_inputBackend->getControlByEnum( InputHelper::KEY_C )->getDelta() >= 0.5 )
	{
		m_leftStickCorrection[0] = -p_input.horizontalInput;
		m_leftStickCorrection[1] = -p_input.verticalInput;
		m_rightStickCorrection[0] = -p_input.strafeHorizontalInput;
		m_rightStickCorrection[1] = -p_input.strafeVerticalInput;
	}

	// Update the analogue sticks for anttweakbar.
	m_leftStickDir[0] = p_input.horizontalInput;
	m_leftStickDir[1] = p_input.verticalInput;
	m_leftStickDir[2] = 0.5f;

	m_rightStickDir[0] = p_input.strafeHorizontalInput;
	m_rightStickDir[1] = p_input.strafeVerticalInput;
	m_rightStickDir[2] = 0.5f;

	// Update the corrected values for the sticks, in for anttweakbar.
	m_leftStickDirWithCorrection[0] = p_input.horizontalInput + m_leftStickCorrection[0];
	m_leftStickDirWithCorrection[1] = p_input.verticalInput + m_leftStickCorrection[1];
	m_leftStickDirWithCorrection[2] = 0.5f;

	m_rightStickDirWithCorrection[0] = p_input.strafeHorizontalInput + m_rightStickCorrection[0];
	m_rightStickDirWithCorrection[1] = p_input.strafeVerticalInput + m_rightStickCorrection[1];
	m_rightStickDirWithCorrection[2] = 0.5f;
}
