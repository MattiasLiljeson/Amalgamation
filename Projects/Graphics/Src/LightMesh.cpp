#include "LightMesh.h"

LightMesh::LightMesh(Buffer<PVertex>* p_mesh, Buffer<SkeletonMappingVertex>* p_skeletonVertexBuffer, Buffer<DIndex>* p_indices)
{
	m_vertexBuffer	= p_mesh;
	m_indexBuffer	= p_indices;
	m_skeletonVertexBuffer = p_skeletonVertexBuffer;
}

LightMesh::~LightMesh()
{
	/************************************************************************/
	/* THERE SHOULDN'T BE AN DELETION HERE IN THE FINAL VERSION				*/
	/************************************************************************/
	// Why? Who wrote this? /Mattias L
	delete m_vertexBuffer;
	delete m_skeletonVertexBuffer;
	delete m_indexBuffer;
}

Buffer<PVertex>* LightMesh::getVertexBuffer()
{
	return m_vertexBuffer;
}
Buffer<SkeletonMappingVertex>* LightMesh::getSkeletonVertexBuffer()
{
	return m_skeletonVertexBuffer;
}

Buffer<DIndex>* LightMesh::getIndexBuffer()
{
	return m_indexBuffer;
}

MaterialInfo LightMesh::getMaterialInfo()
{
	return m_materialInfo;
}

void LightMesh::setTextureId( const MaterialInfo& p_materialInfo )
{
	m_materialInfo = p_materialInfo;
}