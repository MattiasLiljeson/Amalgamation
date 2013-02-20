#include "CameraSystem.h"
#include "CameraInfo.h"
#include "GraphicsBackendSystem.h"
#include "InputBackendSystem.h"
#include "LookAtEntity.h"
#include "Transform.h"
#include <control.h>

CameraSystem::CameraSystem( GraphicsBackendSystem* p_gfxBackend ) : 
			  EntitySystem( SystemType::CameraSystem, 2,
							ComponentType::ComponentTypeIdx::CameraInfo,
							ComponentType::ComponentTypeIdx::Transform)
{
	m_gfxBackend = p_gfxBackend;
	m_renderFromShadowCamera = false;
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
		auto inputSystem = static_cast<InputBackendSystem*>(
			m_world->getSystem(SystemType::InputBackendSystem));

		if ((inputSystem->getControlByEnum(InputHelper::KeyboardKeys_M))->getDelta() > 0.5f){
			m_renderFromShadowCamera = !m_renderFromShadowCamera;
		}

		bool noValidCameraFound = true;
		for(unsigned int i=0; i<p_entities.size(); i++ ){
			if(noValidCameraFound)
				noValidCameraFound = selectPlayerCamera(p_entities[i]);
		}
		if(noValidCameraFound){
			updateRenderSceneInfo(p_entities[0]);
			m_playerCamera = p_entities[0];
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
	
	//AglVector3::normalize(lookTarget);
	//AglVector3::normalize(up);
	
	InputBackendSystem* input = static_cast<InputBackendSystem*>(m_world->getSystem(SystemType::InputBackendSystem));
	if (input->getDeltaByEnum(InputHelper::KeyboardKeys_NUMPAD_1))
		viewProjInv = viewProjInv;
	AglVector3 w = lookTarget-position;
	w.normalize();
	AglVector3 u = AglVector3::crossProduct(up, w);
	u.normalize();
	AglVector3 v = AglVector3::crossProduct(w, u);
	v.normalize();

	RendererSceneInfo sceneInfo;
	sceneInfo.view		= view;
	sceneInfo.viewProj	= viewProj;
	sceneInfo.cameraPos = position;
	sceneInfo.cameraForward = w;
	sceneInfo.cameraUp = v;
	sceneInfo.viewProjInv = viewProjInv;
	sceneInfo.renderTargetDimensions = m_gfxBackend->getWindowSize();
	sceneInfo.farPlane = camInfo->m_farPlane;
	sceneInfo.nearPlane = camInfo->m_nearPlane;

	// sets up certain "global" scene data
	GraphicsWrapper* gfxWrapper = m_gfxBackend->getGfxWrapper();
	gfxWrapper->updateRenderSceneInfo(sceneInfo);
}

void CameraSystem::setRenderSceneInfoUsingPlayerCamera()
{
	updateRenderSceneInfo(m_playerCamera);
}

bool CameraSystem::selectPlayerCamera( Entity* p_camera )
{
	if(m_renderFromShadowCamera){
		if(p_camera->getComponent(ComponentType::TAG_ShadowCamera) != NULL){
			updateRenderSceneInfo(p_camera);
			m_playerCamera = p_camera;
			return false;
		}
	}
	else if( p_camera->getComponent(ComponentType::TAG_MainCamera) != NULL){
		updateRenderSceneInfo(p_camera);
		m_playerCamera = p_camera;
		return false;
	}
	return true;
}
