#include "ShipControllerSystem.h"

ShipControllerSystem::ShipControllerSystem( InputBackendSystem* p_inputBackend ) : 
					  EntitySystem( SystemType::ShipControllerSystem, 2,
									ComponentType::ComponentTypeIdx::ShipController,
									ComponentType::ComponentTypeIdx::Transform)
{
	m_inputBackend = p_inputBackend;
}

ShipControllerSystem::~ShipControllerSystem()
{

}

void ShipControllerSystem::initialize()
{
	m_horizontalPositive= m_inputBackend->getControlByEnum( InputHelper::X_POSITIVE );
	m_horizontalNegative= m_inputBackend->getControlByEnum( InputHelper::X_NEGATIVE );
	m_verticalPositive	= m_inputBackend->getControlByEnum( InputHelper::Y_POSITIVE );
	m_verticalNegative	= m_inputBackend->getControlByEnum( InputHelper::Y_NEGATIVE );

	m_roll		= m_inputBackend->getControlByEnum( InputHelper::KEY_L );
	m_thrust	= m_inputBackend->getControlByEnum( InputHelper::BTN_B );

	m_strafeHorizontalPositive	= m_inputBackend->getControlByEnum( InputHelper::KEY_L );
	m_strafeHorizontalNegative	= m_inputBackend->getControlByEnum( InputHelper::KEY_L );
	m_strafeVerticalPositive	= m_inputBackend->getControlByEnum( InputHelper::KEY_L );
	m_strafeVerticalNegative	= m_inputBackend->getControlByEnum( InputHelper::KEY_L );
}

void ShipControllerSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();
	// Input controls
	double horizontalInput = m_horizontalPositive->getStatus() - m_horizontalNegative->getStatus();
	double verticalInput = m_verticalPositive->getStatus() - m_verticalNegative->getStatus();
	double thrustInput = m_thrust->getStatus();
	double strafeHorizontalInput = 0.0;
	double strafeVerticalInput = 0.0;

	for(unsigned int i=0; i<p_entities.size(); i++ )
	{

		ShipController* controller = static_cast<ShipController*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::ShipController ) );

		Transform* transform = static_cast<Transform*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		// Calc rotation from player input
		float xangle = -verticalInput/* - Input.GetAxis("Mouse Y")*/;
		float yangle = horizontalInput/* + Input.GetAxis("Mouse X")*/;
		float zangle = 0.0f;
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

		// DEBUGPRINT(( (toString(thrustVec.x)+string(" ")+toString(thrustVec.y)+string(" ")+toString(thrustVec.z)+string("\n")).c_str() ));

		// DEBUGPRINT(( (toString(horizontalInput)+string("\n")).c_str() ));

		transform->setTranslation(transform->getTranslation()+thrustVec);


		// Apply force and torque
		// rigidbody.AddForce(m_thrustVec,ForceMode.Acceleration);
		// rigidbody.AddTorque(transform.rotation * m_inputAngles, ForceMode.Acceleration);

		//////////////////////////////////////////////////////////////////////////
	}
}

