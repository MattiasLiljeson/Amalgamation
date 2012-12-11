#include "CameraSystem.h"

// The include below can't be in h-file. Causes linker error.
#include <CamMatrixerUtil.h>

CameraSystem::CameraSystem( GraphicsBackendSystem* p_gfxBackend ): EntitySystem( SystemType::CameraSystem, 1,
										   ComponentType::ComponentTypeIdx::CameraInfo )
{
	m_gfxBackend = p_gfxBackend;
}


CameraSystem::~CameraSystem()
{
}

void CameraSystem::initialize()
{
	m_ticker = 0.0f;
}

void CameraSystem::processEntities( const vector<Entity*>& p_entities )
{
	for( int i=0; i<p_entities.size(); i++ )
	{
		float dt = m_world->getDelta();
		m_ticker += dt;
		
		Component* comp = m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::ComponentTypeIdx::CameraInfo);

		CameraInfo* camInfo = static_cast<CameraInfo*>(comp);
		// HACK: temp animation
		//camInfo->m_pos.z = -1.0f-sin(m_ticker);
		//camInfo->m_pos.x = -1.0f-sin(m_ticker);
		camInfo->m_pos.y = -1.0f-cos(m_ticker);

		MatrixHelper::SetLookAtMatrix( camInfo->m_viewMatrix, camInfo->m_pos, camInfo->m_lookAt, camInfo->m_up );

		camInfo->m_camMatrix = AglMatrix::transpose( AglMatrix::identityMatrix() * camInfo->m_viewMatrix * camInfo->m_projMatrix );
		RendererSceneInfo tempSceneInfo;
		for( int n=0; n<16; n++ )
			tempSceneInfo.viewProjectionMatrix[n] = camInfo->m_camMatrix[n];

		// sets up certain "global" scene data
		GraphicsWrapper* gfxWrapper = m_gfxBackend->getGfxWrapper();
		gfxWrapper->setSceneInfo(tempSceneInfo);
	}
}
