#include "ThrustComponent.h"

ThrustComponent::ThrustComponent() : Component( ComponentType::ThrustComponent)
{
	thrustVector = AglVector3::zero();
	angularVector = AglVector3::zero();
}

ThrustComponent::~ThrustComponent()
{

}
