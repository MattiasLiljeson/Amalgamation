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
		m_ticker += m_world->getDelta();

		CameraInfo* camInfo = static_cast<CameraInfo*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::CameraInfo ) );

		Transform* transform = static_cast<Transform*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		// HACK: temp animation
		//camInfo->m_pos.z = -1.0f-sin(m_ticker);
		//camInfo->m_pos.x = -1.0f-sin(m_ticker);
		//camInfo->m_pos.y = -1.0f-cos(m_ticker);

		AglMatrix view;
		MatrixHelper::SetLookAtMatrix(
			view,
			transform->getTranslation(),
			camInfo->m_lookAt,
			camInfo->m_up );

		AglMatrix viewProj = AglMatrix::identityMatrix() ;
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
