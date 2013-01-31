#include "ShadowSystem.h"

ShadowSystem::ShadowSystem() : EntitySystem( SystemType::ShadowSystem, 3, 
											ComponentType::CameraInfo,
											ComponentType::TAG_ShadowCamera,
											ComponentType::Transform){
	
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
