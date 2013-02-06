#include "ShadowSystem.h"
#include <AglMatrix.h>
#include "CameraInfo.h"
#include "Transform.h"

ShadowSystem::ShadowSystem() : EntitySystem( SystemType::ShadowSystem, 4, 
											ComponentType::CameraInfo,
											ComponentType::TAG_ShadowCamera,
											ComponentType::Transform,
											ComponentType::LightsComponent){
	
}

ShadowSystem::~ShadowSystem()
{

}

void ShadowSystem::processEntities( const vector<Entity*>& p_entities ){

	m_shadowCameras.clear();

	for (unsigned int i = 0; i < p_entities.size(); i++){
		m_shadowCameras.push_back(p_entities[i]);
	}
}

unsigned int ShadowSystem::getNumberOfShadowCameras() const{
	return m_shadowCameras.size();
}

Entity* ShadowSystem::getShadowEntity( const unsigned int p_index){
	return m_shadowCameras[p_index];
}

AglMatrix ShadowSystem::getViewProjection( const unsigned int p_index ){
	CameraInfo* cameraInfo = static_cast<CameraInfo*>(
		m_shadowCameras[p_index]->getComponent(ComponentType::CameraInfo));

	Transform* transform = static_cast<Transform*>(
		m_shadowCameras[p_index]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

	AglVector3 position = transform->getTranslation();
	AglQuaternion rotation = transform->getRotation();
	AglVector3 lookTarget = position+transform->getMatrix().GetForward();
	AglVector3 up = transform->getMatrix().GetUp();

	// Construct view matrix
	AglMatrix view = AglMatrix::createViewMatrix(
		position,
		lookTarget,
		up);

	// Rendering preparations
	AglMatrix viewProj = AglMatrix::identityMatrix();
	viewProj = view * cameraInfo->m_projMat;
	viewProj = AglMatrix::transpose( viewProj );

	return viewProj;
}
