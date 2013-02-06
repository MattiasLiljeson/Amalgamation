#include "LookAtSystem.h"

#include "CameraSystem.h"
#include "CameraInfo.h"
#include "GraphicsBackendSystem.h"
#include "InputBackendSystem.h"
#include "LookAtEntity.h"
#include "Transform.h"
#include "GameplayTags.h"
#include "ValueClamp.h"
#include "PhysicsBody.h"

LookAtSystem::LookAtSystem() : 
EntitySystem( SystemType::LookAtSystem, 2,
			 ComponentType::ComponentTypeIdx::LookAtEntity,
			 ComponentType::ComponentTypeIdx::Transform)
{

}


LookAtSystem::~LookAtSystem()
{
}

void LookAtSystem::initialize()
{

}

void LookAtSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();

	for(unsigned int i=0; i<p_entities.size(); i++ )
	{

		Transform* transform = static_cast<Transform*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		// optional component for lookat
		LookAtEntity* lookAt= static_cast<LookAtEntity*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::LookAtEntity) );

		// optional lookAt tags
		LookAtFollowMode_TAG* lookAtFollow=NULL;
		LookAtOrbitMode_TAG* lookAtOrbit=NULL;

		Component* t = p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::TAG_LookAtFollowMode );
		if (t!=NULL)
			lookAtFollow = static_cast<LookAtFollowMode_TAG*>(t);


		t = p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::TAG_LookAtOrbitMode );
		if (t!=NULL)
			lookAtOrbit = static_cast<LookAtOrbitMode_TAG*>(t);


		// Retrieve initial info
		AglVector3 position;
		if (transform) position = transform->getTranslation();
		AglQuaternion rotation = transform->getRotation();
		
		// Extract look-at entity and its transform
		Entity* targetEntity = m_world->getEntity(lookAt->getEntityId());
		Transform* targetTransform = static_cast<Transform*>(
			targetEntity->getComponent(ComponentType::ComponentTypeIdx::Transform));


		AglVector3 lookTargetPos;
		if (targetTransform)
		{
			lookTargetPos = targetTransform->getTranslation();
		}



		// Follow behaviour
		if (lookAtFollow)
		{		
			// Set up look-at vars for the view matrix
			// Create offset vector from look-at component in the space of the target
			AglVector3 offset = lookAt->getFollowPositionOffset();
			offset.transformNormal(targetTransform->getMatrix());
			// update transform

			// lerp
			if (lookAt->getMoveSpd()*dt<1.0f)
			{
				position = AglVector3::lerp(position,lookTargetPos+offset,
					saturate(lookAt->getMoveSpd()*dt));
			}
			else
			{
				position = lookTargetPos+offset;
			}

			// slerp
			if (lookAt->getRotationSpeed()*dt<1.0f)
			{
				rotation = AglQuaternion::slerp(rotation,targetTransform->getRotation(),
					saturate(lookAt->getRotationSpeed()*dt),true);
				rotation.normalize();
			}
			else
			{
				rotation = targetTransform->getRotation();
			}
			

			// update			
			transform->setRotation( rotation );
			transform->setTranslation( position );
		}
		// orbit behaviour
		else if (lookAtOrbit)
		{
			AglQuaternion interRotation = lookAt->getOrbitOffset();
			AglVector3 move = lookAt->getOrbitMovement()*lookAt->getOrbitRotationSpeed();
			if (move.length()>0.0f)	
				interRotation *= AglQuaternion::constructFromAngularVelocity(move*dt);
			interRotation.normalize();
			lookAt->setOrbitOffset(interRotation);

			rotation = AglQuaternion::slerp(rotation,targetTransform->getRotation()*interRotation,
				saturate(lookAt->getRotationSpeed()*0.7f*dt),true);
			rotation.normalize();

			AglVector3 offset = AglVector3::backward()*lookAt->getOrbitDistance();
			rotation.transformVector(offset);
			position = AglVector3::lerp(position, lookTargetPos + offset,
				saturate(lookAt->getMoveSpd()*3.0f*dt));
			

			// update
			transform->setTranslation( position );
			transform->setRotation( rotation );
		}
		// just lookat behaviour
		else if (!lookAtOrbit && !lookAtFollow)
		{
 			AglVector3 dir = position-lookTargetPos;
 			AglVector3::normalize(dir);
 			transform->setForwardDirection(dir);
		}
		
	}
}
