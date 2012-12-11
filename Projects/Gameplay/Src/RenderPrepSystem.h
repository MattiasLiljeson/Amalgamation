#pragma once

#include "RenderInfo.h"
#include "GraphicsBackendSystem.h"

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


class RenderPrepSystem : public EntitySystem
{
public:
	RenderPrepSystem(  GraphicsBackendSystem* p_gfxBackend  );
	virtual ~RenderPrepSystem();
	virtual void initialize();

	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	GraphicsBackendSystem* m_gfxBackend;

};

