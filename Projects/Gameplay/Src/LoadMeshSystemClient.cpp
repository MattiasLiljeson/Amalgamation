#include "LoadMeshSystemClient.h"

#include "GraphicsBackendSystem.h"
#include "LoadMesh.h"
#include <ModelResource.h>
#include <GraphicsWrapper.h>
#include "Transform.h"
#include "RenderInfo.h"
#include "EntityParent.h"
#include "ConnectionPointSet.h"
#include "BodyInitData.h"
#include "PhysicsBody.h"

LoadMeshSystemClient::LoadMeshSystemClient( GraphicsBackendSystem* p_gfxBackend ) : 
	LoadMeshSystem()
{
	m_gfxBackend = p_gfxBackend;
}


LoadMeshSystemClient::~LoadMeshSystemClient()
{
}

void LoadMeshSystemClient::initialize()
{

}

vector<ModelResource*>* LoadMeshSystemClient::createModels( const string& p_filename, const string& p_filePath, bool p_isPrimitive )
{
	return m_gfxBackend->getGfxWrapper->createModelsFromFile(p_filename,p_filePath,p_isPrimitive);
}
