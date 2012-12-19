#pragma once

#include "RenderInfo.h"
#include "GraphicsBackendSystem.h"
#include "Transform.h"

#include <AglMatrix.h>
#include <AglVector3.h>
#include <ComponentType.h>
#include <DebugUtil.h>
#include <EntitySystem.h>
#include <GraphicsWrapper.h>
#include <TextureParser.h>
#include <Window.h>
#include <Windows.h>
#include <vld.h>
#include "libRocketBackendSystem.h"


class RenderPrepSystem : public EntitySystem
{
public:
	RenderPrepSystem(  GraphicsBackendSystem* p_gfxBackend,
		LibRocketBackendSystem* p_rocketBackend );
	virtual ~RenderPrepSystem();
	virtual void initialize();

	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	vector< vector<InstanceData> > m_instanceLists;
	GraphicsBackendSystem* m_gfxBackend;
	LibRocketBackendSystem* m_rocketBackend;

};

