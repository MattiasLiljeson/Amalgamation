#include "Mesh.h"

Mesh::Mesh(Buffer<PNTTBVertex>* p_mesh , Buffer<SkeletonMappingVertex>* p_skeletonVertexBuffer, Buffer<DIndex>* p_indices)
{
	m_vertexBuffer		= p_mesh;
	m_skeletonVertexBuffer = p_skeletonVertexBuffer;
	m_indexBuffer	= p_indices;
}

Mesh::~Mesh()
{
	/************************************************************************/
	/* THERE SHOULDN'T BE AN DELETION HERE IN THE FINAL VERSION				*/
	/************************************************************************/
	delete m_vertexBuffer;
	delete m_skeletonVertexBuffer;
	delete m_indexBuffer;
}

Buffer<PNTTBVertex>* Mesh::getVertexBuffer()
{
	return m_vertexBuffer;
}

Buffer<SkeletonMappingVertex>* Mesh::getSkeletonVertexBuffer()
{
	return m_skeletonVertexBuffer;
}

Buffer<DIndex>* Mesh::getIndexBuffer()
{
	return m_indexBuffer;
}

MaterialInfo Mesh::getMaterialInfo()
{
	return m_materialInfo;
}

void Mesh::setMaterial( const MaterialInfo& p_materialInfo )
{
	m_materialInfo = p_materialInfo;
}