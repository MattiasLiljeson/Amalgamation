#include "CameraSystem.h"
#include "CameraInfo.h"
#include "GraphicsBackendSystem.h"
#include "InputBackendSystem.h"
#include "LookAtEntity.h"
#include "Transform.h"

CameraSystem::CameraSystem( GraphicsBackendSystem* p_gfxBackend, 
							InputBackendSystem* p_inputBackend ) : 
			  EntitySystem( SystemType::CameraSystem, 2,
							ComponentType::ComponentTypeIdx::CameraInfo,
							ComponentType::ComponentTypeIdx::Transform)
{
	m_gfxBackend = p_gfxBackend;
	m_inputBackend = p_inputBackend;
}


CameraSystem::~CameraSystem()
{
}

void CameraSystem::initialize()
{

}

void CameraSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();
	for(unsigned int i=0; i<p_entities.size(); i++ )
	{

		CameraInfo* camInfo = static_cast<CameraInfo*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::CameraInfo ) );

		Transform* transform = static_cast<Transform*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		// optional component for lookat
		LookAtEntity* lookAt=NULL;
		Component* t = p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::LookAtEntity );
		if (t!=NULL)
			lookAt = static_cast<LookAtEntity*>(t);

		// Retrieve initial info
		AglVector3 position = transform->getTranslation();
		AglQuaternion rotation = transform->getRotation();
		AglVector3 lookTarget = position+transform->getMatrix().GetForward();
		AglVector3 up = transform->getMatrix().GetUp();

		// Prepare lookat values if used
		if (lookAt)
		{
			// Extract look-at entity and its transform
			Entity* targetEntity = m_world->getEntity(lookAt->getEntityId());
			Transform* targetTransform = static_cast<Transform*>(
				targetEntity->getComponent(ComponentType::ComponentTypeIdx::Transform));
			lookTarget = targetTransform->getTranslation();
			// Set up look-at vars for the view matrix
			// Create offset vector from look-at component in the space of the target
			AglVector3 offset = lookAt->getOffset();
			offset.transformNormal(targetTransform->getMatrix());
			// Transform camera up
			up = targetTransform->getMatrix().GetUp();

			// update transform
//			position = AglVector3::lerp(position,lookTarget+offset,
//										lookAt->getMoveSpd()*dt);
//			rotation = AglQuaternion::slerp(rotation,targetTransform->getRotation(),
//											lookAt->getRotationSpeed()*dt);
			position = lookTarget+offset;
			rotation = targetTransform->getRotation();
			rotation.normalize();
		}

		// Construct view matrix
		AglMatrix view = AglMatrix::createViewMatrix(position,
													 lookTarget,
													 up);

		// update of position
		transform->setTranslation( position );
		transform->setRotation( rotation );
		

		// Rendering preparations
		AglMatrix viewProj = AglMatrix::identityMatrix();
		viewProj = view * camInfo->m_projMat;
		viewProj = AglMatrix::transpose( viewProj );
		
		RendererSceneInfo tempSceneInfo;
		for( int i=0; i<16; i++ )
		{
			tempSceneInfo.viewProjectionMatrix[i] = viewProj[i];
		}

		// sets up certain "global" scene data
		GraphicsWrapper* gfxWrapper = m_gfxBackend->getGfxWrapper();
		gfxWrapper->setSceneInfo(tempSceneInfo);
	}
}
