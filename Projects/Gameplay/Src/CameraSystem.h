#pragma once
#include "CameraInfo.h"
#include "GraphicsBackendSystem.h"
#include "InputBackendSystem.h"
#include "LookAtEntity.h"
#include "Transform.h"
#include <EntitySystem.h>

class CameraSystem : public EntitySystem
{
public:
	CameraSystem( GraphicsBackendSystem* p_gfxBackend, 
				  InputBackendSystem* p_inputBackend );
	~CameraSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	GraphicsBackendSystem* m_gfxBackend;
	InputBackendSystem* m_inputBackend;
};

