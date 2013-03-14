#include "ThrustComponent.h"

ThrustComponent::ThrustComponent() : Component( ComponentType::ThrustComponent)
{
	m_resultingThrust = AglVector3::zero();
	m_resultingAngular = AglVector3::zero();
	m_thrustPower = 0.0f;
	m_angularPower = 0.0f;
}

ThrustComponent::~ThrustComponent()
{

}

void ThrustComponent::addThrustVector( const AglVector3& p_thrust, float p_delta )
{
	m_resultingThrust += p_thrust;
	m_resultingThrust -= m_resultingThrust*p_delta;
	m_thrustPower = AglVector3::length(m_resultingThrust);
	
	if(m_thrustPower > (float)POWERCAP)
		m_thrustPower = POWERCAP;
}

void ThrustComponent::addAngularVector( const AglVector3& p_angular, float p_delta )
{
	m_resultingAngular += p_angular;
	m_resultingAngular -= m_resultingAngular*p_delta;
	m_angularPower = AglVector3::length(m_resultingAngular);

	if(m_angularPower > (float)ANGULARCAP)
		m_angularPower = ANGULARCAP;
}
