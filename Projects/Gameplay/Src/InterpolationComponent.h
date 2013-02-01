#pragma once

// =======================================================================================
//                                 InterpolationComponent
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Component for storing data for interpolation of transforms
///        
/// # InterpolationComponent
/// Detailed description.....
/// Created on: 1-2-2013
///---------------------------------------------------------------------------------------

#include <Component.h>
#include <DebugUtil.h>
#include <ToString.h>
#include <AglVector3.h>
#include <AglQuaternion.h>

#include <queue>
using namespace std;

class InterpolationComponent : public Component
{
public:
	struct TransformGoal
	{
		AglVector3 translation;
		AglQuaternion rotation;
		AglVector3 scale;
		float timestamp;
	};

	InterpolationComponent(){m_currentTransformTimestamp=-1.0f;}
	virtual ~InterpolationComponent() {}


	float m_currentTransformTimestamp;
	float m_startTransformTimestamp;
	float m_goalTransformTimestamp;
	AglVector3 m_currentTranslation;
	AglVector3 m_startTranslation;
	AglVector3 m_goalTranslation;
	AglQuaternion m_currentRotation;
	AglQuaternion m_startRotation;
	AglQuaternion m_goalRotation;
	AglVector3 m_currentScale;
	AglVector3 m_startScale;
	AglVector3 m_goalScale;

	queue<TransformGoal> m_transformBuffer;
};