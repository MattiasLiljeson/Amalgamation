#include "LightMesh.h"

LightMesh::LightMesh(Buffer<PVertex>* p_mesh , Buffer<DIndex>* p_indices)
{
	m_vertexBuffer	= p_mesh;
	m_indexBuffer	= p_indices;
}

LightMesh::~LightMesh()
{
	/************************************************************************/
	/* THERE SHOULDN'T BE AN DELETION HERE IN THE FINAL VERSION				*/
	/************************************************************************/
	// Why? Who wrote this? /Mattias L
	delete m_vertexBuffer;
	delete m_indexBuffer;
}

Buffer<PVertex>* LightMesh::getVertexBuffer()
{
	return m_vertexBuffer;
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