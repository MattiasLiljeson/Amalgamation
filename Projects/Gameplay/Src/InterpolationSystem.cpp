#include "InterpolationSystem.h"
#include <DebugUtil.h>

// Components
#include "InterpolationComponent.h"
#include "Transform.h"

#define FORCE_VS_OUTPUT

InterpolationSystem::InterpolationSystem( )
	: EntitySystem( SystemType::InterpolationSystem, 2,
	ComponentType::InterpolationComponent, ComponentType::Transform )
{

}

InterpolationSystem::~InterpolationSystem()
{
}

void InterpolationSystem::processEntities( const vector<Entity*>& p_entities )
{

	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		InterpolationComponent* interpolate = NULL;
		interpolate = static_cast<InterpolationComponent*>(p_entities[i]->getComponent(
			ComponentType::InterpolationComponent));

		Transform* transform = NULL;
		transform = static_cast<Transform*>(p_entities[i]->getComponent(
			ComponentType::Transform));

		// handleTransformInterpolation(interpolate,transform);
	}
}

void InterpolationSystem::initialize()
{

}


void InterpolationSystem::handleTransformInterpolation( InterpolationComponent* p_interpolation, 
														   Transform* p_transform )
{
	// fetch data
	// init
	float dt = m_world->getDelta();
	if (!p_interpolation->m_transformBuffer.empty() &&
		p_interpolation->m_currentTransformTimestamp<0.0f) // if no data gotten yet, init
	{
		InterpolationComponent::TransformGoal front = p_interpolation->m_transformBuffer.front();
		p_interpolation->m_transformBuffer.pop();		
		p_interpolation->m_currentTransformTimestamp = front.timestamp;
		p_interpolation->m_startTransformTimestamp = front.timestamp;
		p_interpolation->m_goalTransformTimestamp = front.timestamp;

		p_interpolation->m_startTranslation = front.translation;
		p_interpolation->m_currentTranslation = front.translation;

		p_interpolation->m_startRotation = front.rotation;
		p_interpolation->m_currentRotation = front.rotation;

		p_interpolation->m_startScale = front.scale;
		p_interpolation->m_currentScale = front.scale;

	}
	if (!p_interpolation->m_transformBuffer.empty() &&
		p_interpolation->m_currentTransformTimestamp>0.0f) // get goal
	{
		// get new data if we have catched up
		// while timestamp batched at same time and there is still data
		while (p_interpolation->m_currentTransformTimestamp>=p_interpolation->m_goalTransformTimestamp 
				&& !p_interpolation->m_transformBuffer.empty())					
		{
			p_interpolation->m_startTranslation = p_interpolation->m_goalTranslation; // set our start values
			p_interpolation->m_startRotation = p_interpolation->m_goalRotation;
			p_interpolation->m_startScale = p_interpolation->m_goalScale;

			InterpolationComponent::TransformGoal front = p_interpolation->m_transformBuffer.front(); // fetch new front
			p_interpolation->m_goalTranslation = front.translation; // set our goal to the new front
			p_interpolation->m_goalRotation = front.rotation;
			p_interpolation->m_goalScale = front.scale;

			p_interpolation->m_startTransformTimestamp = p_interpolation->m_currentTransformTimestamp; // set set start time to current
			p_interpolation->m_goalTransformTimestamp = front.timestamp; // set our goal timestamp to the new front
			p_interpolation->m_transformBuffer.pop(); // pop old front
		}
	}

	DEBUGPRINT(( ("\nQueue size: "+toString(p_interpolation->m_transformBuffer.size())+"\n").c_str() ));



	// interpolate
	if (p_interpolation->m_currentTransformTimestamp<p_interpolation->m_goalTransformTimestamp)
	{

		float startT = p_interpolation->m_startTransformTimestamp;
		float endT = p_interpolation->m_goalTransformTimestamp;		
		
		p_interpolation->m_currentTransformTimestamp += dt;
		float currentT = p_interpolation->m_currentTransformTimestamp;

		float t = (currentT-startT) / max(0.00001f,(endT-startT));
		DEBUGPRINT(( ("\nCurrent lerp: "+toString(t)+"\n").c_str() ));
		AglVector3 lerpedTranslation = AglVector3::lerp(p_interpolation->m_startTranslation,
			p_interpolation->m_goalTranslation,t);	
		AglQuaternion slerpedRotation = AglQuaternion::slerp(p_interpolation->m_startRotation,
			p_interpolation->m_goalRotation,t,true);	
		slerpedRotation.normalize();
		AglVector3 lerpedScale= AglVector3::lerp(p_interpolation->m_startScale,
			p_interpolation->m_goalScale,t);
		p_interpolation->m_currentTranslation = lerpedTranslation;
		p_interpolation->m_currentRotation = slerpedRotation;
		p_interpolation->m_currentScale = lerpedScale;
	}
	else
	{
		p_interpolation->m_currentTranslation = p_interpolation->m_goalTranslation;
		p_interpolation->m_currentRotation = p_interpolation->m_goalRotation;
		p_interpolation->m_currentScale = p_interpolation->m_goalScale;
	}


	// update
	p_transform->setTranslation(p_interpolation->m_currentTranslation);
	p_transform->setRotation(p_interpolation->m_currentRotation);
	p_transform->setScale(p_interpolation->m_currentScale);

}
