#include "CameraSystem.h"
#include "CameraInfo.h"
#include "GraphicsBackendSystem.h"
#include "InputBackendSystem.h"
#include "LookAtEntity.h"
#include "Transform.h"

CameraSystem::CameraSystem( GraphicsBackendSystem* p_gfxBackend ) : 
			  EntitySystem( SystemType::CameraSystem, 2,
							ComponentType::ComponentTypeIdx::CameraInfo,
							ComponentType::ComponentTypeIdx::Transform)
{
	m_gfxBackend = p_gfxBackend;
}


CameraSystem::~CameraSystem()
{
}

void CameraSystem::initialize()
{

}

void CameraSystem::processEntities( const vector<Entity*>& p_entities )
{
	if(p_entities.size() > 0){
		bool mainCameraDontExist = true;
		float dt = m_world->getDelta();
		for(unsigned int i=0; i<p_entities.size(); i++ )
		{
			if( p_entities[i]->getComponent(ComponentType::TAG_MainCamera) != NULL){
			
				mainCameraDontExist = false;
				updateRenderSceneInfo(p_entities[i]);
			}
		}
		if(mainCameraDontExist){
			updateRenderSceneInfo(p_entities[0]);
		}
	}
}

void CameraSystem::updateRenderSceneInfo( Entity* p_entity )
{
	CameraInfo* camInfo = static_cast<CameraInfo*>(
		p_entity->getComponent( ComponentType::ComponentTypeIdx::CameraInfo ) );

	Transform* transform = static_cast<Transform*>(
		p_entity->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

	// Retrieve initial info
	AglVector3 position = transform->getTranslation();
	AglQuaternion rotation = transform->getRotation();
	AglVector3 lookTarget = position+transform->getMatrix().GetForward();
	AglVector3 up = transform->getMatrix().GetUp();

	// Construct view matrix
	AglMatrix view = AglMatrix::createViewMatrix(position,
		lookTarget,
		up);

	// Rendering preparations
	AglMatrix viewProj = AglMatrix::identityMatrix();
	viewProj = view * camInfo->m_projMat;
	AglMatrix viewProjInv = AglMatrix::inverse(viewProj);
	viewProj = AglMatrix::transpose( viewProj );
	viewProjInv = AglMatrix::transpose(viewProjInv);
	AglVector3::normalize(lookTarget);
	AglVector3::normalize(up);

	RendererSceneInfo sceneInfo;
	sceneInfo.viewProj = viewProj;
	sceneInfo.cameraPos = position;
	sceneInfo.cameraForward = lookTarget;
	sceneInfo.cameraUp = up;
	sceneInfo.viewProjInv = viewProjInv;
	sceneInfo.renderTargetDimensions = m_gfxBackend->getWindowSize();
	sceneInfo.farPlane = camInfo->m_farPlane;
	sceneInfo.nearPlane = camInfo->m_nearPlane;

	// sets up certain "global" scene data
	GraphicsWrapper* gfxWrapper = m_gfxBackend->getGfxWrapper();
	gfxWrapper->updateRenderSceneInfo(sceneInfo);
}
