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
									ComponentType::ComponentTypeIdx::Transform
									/*, ComponentType::ComponentTypeIdx::PhysicsBody*/)
{
	m_inputBackend = p_inputBackend;
	m_physics = p_physicsSystem;
	m_client = p_client;

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

void ShipControllerSystem::initialize()
{
	m_horizontalPositive	= m_inputBackend->getInputControl( "THUMB_LX_POSITIVE" );
	m_horizontalNegative	= m_inputBackend->getInputControl( "THUMB_LX_NEGATIVE" );
	m_verticalPositive		= m_inputBackend->getInputControl( "THUMB_LY_POSITIVE" );
	m_verticalNegative		= m_inputBackend->getInputControl( "THUMB_LY_NEGATIVE" );

	m_rollRight		= m_inputBackend->getInputControl( "SHOULDER_PRESS_R" );
	m_rollLeft		= m_inputBackend->getInputControl( "SHOULDER_PRESS_L" );
	m_thrust		= m_inputBackend->getInputControl( "TRIGGER_R" );

	m_strafeHorizontalPositive	= m_inputBackend->getInputControl( "THUMB_RX_POSITIVE" );
	m_strafeHorizontalNegative	= m_inputBackend->getInputControl( "THUMB_RX_NEGATIVE" );
	m_strafeVerticalPositive	= m_inputBackend->getInputControl( "THUMB_RY_POSITIVE" );
	m_strafeVerticalNegative	= m_inputBackend->getInputControl( "THUMB_RY_NEGATIVE" );

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

}

void ShipControllerSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();
	// Input controls setup
	double hPositive = m_horizontalPositive->getStatus(),
		   hNegative = m_horizontalNegative->getStatus(),
		   vPositive = m_verticalPositive->getStatus(),
		   vNegative = m_verticalNegative->getStatus(),
		   shPositive = m_strafeHorizontalPositive->getStatus(),
		   shNegative = m_strafeHorizontalNegative->getStatus(),
		   svPositive = m_strafeVerticalPositive->getStatus(),
		   svNegative = m_strafeVerticalNegative->getStatus();


	// Store raw float data
	float horizontalInput = (float)(hPositive - hNegative);
	float verticalInput = (float)(vPositive - vNegative);
	float rollInput =  (float)(m_rollLeft->getStatus()-m_rollRight->getStatus());
	float thrustInput = (float)(m_thrust->getStatus());
	float strafeHorizontalInput = (float)(shPositive - shNegative);
	float strafeVerticalInput = (float)(svPositive - svNegative);
	float sensitivityMult = 1.0f;

	// Calibrate the Gamepad's analogue sticks when pressing the C key.
	if( m_inputBackend->getControlByEnum( InputHelper::KEY_C )->getDelta() >= 0.5 )
	{
		m_leftStickCorrection[0] = -horizontalInput;
		m_leftStickCorrection[1] = -verticalInput;
		m_rightStickCorrection[0] = -strafeHorizontalInput;
		m_rightStickCorrection[1] = -strafeVerticalInput;
	}

	// Update the analogue sticks for anttweakbar.
	m_leftStickDir[0] = horizontalInput;
	m_leftStickDir[1] = verticalInput;
	m_leftStickDir[2] = 0;

	m_rightStickDir[0] = strafeHorizontalInput;
	m_rightStickDir[1] = strafeVerticalInput;
	m_rightStickDir[2] = 0;

	// Update the corrected values for the sticks, in for anttweakbar.
	m_leftStickDirWithCorrection[0] = horizontalInput + m_leftStickCorrection[0];
	m_leftStickDirWithCorrection[1] = verticalInput + m_leftStickCorrection[1];
	m_leftStickDirWithCorrection[2] = 0;

	m_rightStickDirWithCorrection[0] = strafeHorizontalInput + m_rightStickCorrection[0];
	m_rightStickDirWithCorrection[1] = strafeVerticalInput + m_rightStickCorrection[1];
	m_rightStickDirWithCorrection[2] = 0;

	// Apply correction vectors to the analogue sticks.
	horizontalInput += m_leftStickCorrection[0];
	verticalInput += m_leftStickCorrection[1];

	// Apply a threshold value to eliminate some of the analogue stick noise.
	if( abs(horizontalInput) < m_controllerEpsilon )
		horizontalInput = 0;
	if( abs(verticalInput) < m_controllerEpsilon )
		verticalInput = 0;

	if( abs(strafeHorizontalInput) < m_controllerEpsilon )
		strafeHorizontalInput = 0;
	if( abs(strafeVerticalInput) < m_controllerEpsilon )
		strafeVerticalInput = 0;

	for(unsigned int i=0; i<p_entities.size(); i++ )
	{

		ShipController* controller = static_cast<ShipController*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::ShipController ) );

		Transform* transform = static_cast<Transform*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

//		PhysicsBody* physicsBody = static_cast<PhysicsBody*>(
//			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::PhysicsBody ) );

		// Calc rotation from player input
		float xangle = verticalInput * sensitivityMult/* - Input.GetAxis("Mouse Y")*/;
		float yangle = horizontalInput * sensitivityMult/* + Input.GetAxis("Mouse X")*/;
		float zangle = rollInput * sensitivityMult;
		AglVector3 inputAngles(xangle,yangle,zangle);

		// Turning multiplier
		float  turnSpeed = controller->getTurningSpeed() * dt;
		// Thrust multiplier
		float  thrustPower = controller->getThrustPower() * dt;

		// Calc translation from player input
		AglVector3 thrustVec;
		thrustVec += transform->getMatrix().GetForward() * thrustInput * thrustPower;
		thrustVec += transform->getMatrix().GetRight() * strafeHorizontalInput * thrustPower;
		thrustVec += transform->getMatrix().GetUp()	 * strafeVerticalInput * thrustPower;

		// Calc rotation from player input
		AglVector3 angularVec=inputAngles*turnSpeed;
		AglQuaternion quat = transform->getRotation();
		quat.transformVector(angularVec);

		//DEBUGPRINT(( (toString(angularVec.x)+string(" ")+toString(angularVec.y)+
		//	string(" ")+toString(angularVec.z)+string("\n")).c_str() ));

		// DEBUGPRINT(( (toString(horizontalInput)+string("\n")).c_str() ));

		// transform->setTranslation(transform->getTranslation()+thrustVec);


		//
		// Applying (local client) thrust and impulses, to the physics system.
		// At the moment this is totally replaced by the network thrust-thingy.
		// Later we will probably have both client and server physics.
		//
//		m_physics->applyImpulse(physicsBody->m_id,thrustVec,angularVec);


		//
		// Applying networked thrust and impulses, to the server's own physics system.
		//
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


