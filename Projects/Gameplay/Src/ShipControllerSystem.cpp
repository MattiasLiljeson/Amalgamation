#include "ShipControllerSystem.h"

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
		thrustPacket << (char)NetworkType::Ship << (char)PacketType::PlayerInput 
			<< thrustVec << angularVec;
		m_client->sendPacket( thrustPacket );

	}
}

