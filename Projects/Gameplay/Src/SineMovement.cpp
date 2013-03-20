#include "SineMovement.h"

SineMovement::SineMovement()
	: Component(ComponentType::SineMovement)
{
	vector = AglVector3::zero();
	cycleTime = 1.0f;
	radian = 0.0f;
	originTranslation = AglVector3::zero();
}

SineMovement::SineMovement( AglVector3 p_vector, float p_radian /*= 0.0f*/, float p_cycleTime /*= 1.0f*/ )
	: Component(ComponentType::SineMovement)
{
	vector = p_vector;
	cycleTime = p_cycleTime;
	radian = p_radian;
	originTranslation = AglVector3::zero();
}
