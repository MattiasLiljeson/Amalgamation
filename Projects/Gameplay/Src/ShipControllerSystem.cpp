#include "ShipControllerSystem.h"

ShipControllerSystem::ShipControllerSystem( InputBackendSystem* p_inputBackend,
										    PhysicsSystem* p_physicsSystem ) : 
					  EntitySystem( SystemType::ShipControllerSystem, 3,
									ComponentType::ComponentTypeIdx::ShipController,
									ComponentType::ComponentTypeIdx::Transform,
									ComponentType::ComponentTypeIdx::PhysicsBody)
{
	m_inputBackend = p_inputBackend;
	m_physics = p_physicsSystem;
}

ShipControllerSystem::~ShipControllerSystem()
{

}

void ShipControllerSystem::initialize()
{
	m_horizontalPositive	= m_inputBackend->getInputControl("Gamepad LX positive");
	m_horizontalNegative	= m_inputBackend->getInputControl("Gamepad LX negative");
	m_verticalPositive		= m_inputBackend->getInputControl("Gamepad LY positive");
	m_verticalNegative		= m_inputBackend->getInputControl("Gamepad LY negative");

	m_rollRight		= m_inputBackend->getInputControl("Gamepad R shoulder button");
	m_rollLeft		= m_inputBackend->getInputControl("Gamepad L shoulder button");
	m_thrust		= m_inputBackend->getInputControl("Gamepad R analog trigger");

	m_strafeHorizontalPositive	= m_inputBackend->getInputControl("L");
	m_strafeHorizontalNegative	= m_inputBackend->getInputControl("L");
	m_strafeVerticalPositive	= m_inputBackend->getInputControl("L");
	m_strafeVerticalNegative	= m_inputBackend->getInputControl("L");
}

void ShipControllerSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();
	// Input controls
	double horizontalInput = m_horizontalPositive->getStatus() - m_horizontalNegative->getStatus();
	double verticalInput = m_verticalPositive->getStatus() - m_verticalNegative->getStatus();
	double rollInput =  m_rollLeft->getStatus()-m_rollRight->getStatus();
	double thrustInput = m_thrust->getStatus();
	double strafeHorizontalInput = 0.0;
	double strafeVerticalInput = 0.0;
	double sensitivityMult = 1.0;

	for(unsigned int i=0; i<p_entities.size(); i++ )
	{

		ShipController* controller = static_cast<ShipController*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::ShipController ) );

		Transform* transform = static_cast<Transform*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		PhysicsBody* physicsBody = static_cast<PhysicsBody*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::PhysicsBody ) );

		// Calc rotation from player input
		float xangle = verticalInput * sensitivityMult/* - Input.GetAxis("Mouse Y")*/;
		float yangle = horizontalInput * sensitivityMult/* + Input.GetAxis("Mouse X")*/;
		float zangle = rollInput * sensitivityMult;
		AglVector3 inputAngles(xangle,yangle,zangle);

		// Turning multiplier
		float  turnSpeed = controller->getTurningSpeed() * dt;
		// Thrust multiplier
		float  thrustPower = controller->getThrustPower() * dt;

		// Create rotation quaternion from angles and turn speed
		// AglQuaternion rotation = AglQuaternion::constructFromAngularVelocity(inputAngles*turnSpeed);

		// transform->setRotation(transform->getRotation()+rotation);

		// Calc translation from player input
		AglVector3 thrustVec;
		thrustVec += transform->getMatrix().GetForward() * thrustInput * thrustPower;
		thrustVec += transform->getMatrix().GetRight() * strafeHorizontalInput * thrustPower;
		thrustVec += transform->getMatrix().GetUp()	 * strafeVerticalInput * thrustPower;

		AglVector3 angularVec=inputAngles*turnSpeed;
		AglQuaternion quat = transform->getRotation();
		quat.transformVector(angularVec);

		DEBUGPRINT(( (toString(angularVec.x)+string(" ")+toString(angularVec.y)+
			string(" ")+toString(angularVec.z)+string("\n")).c_str() ));

		// DEBUGPRINT(( (toString(horizontalInput)+string("\n")).c_str() ));

		// transform->setTranslation(transform->getTranslation()+thrustVec);
		m_physics->applyImpulse(physicsBody->m_id,thrustVec,angularVec);

		// Apply force and torque
		// rigidbody.AddForce(m_thrustVec,ForceMode.Acceleration);
		// rigidbody.AddTorque(transform.rotation * m_inputAngles, ForceMode.Acceleration);

		//////////////////////////////////////////////////////////////////////////
	}
}

