#include "Mesh.h"

Mesh::Mesh(Buffer<PNTTBVertex>* p_mesh , Buffer<DIndex>* p_indices)
{
	m_vertexBuffer		= p_mesh;
	m_indexBuffer	= p_indices;
}

Mesh::~Mesh()
{
	/************************************************************************/
	/* THERE SHOULDN'T BE AN DELETION HERE IN THE FINAL VERSION				*/
	/************************************************************************/
	delete m_vertexBuffer;
	delete m_indexBuffer;
}

Buffer<PNTTBVertex>* Mesh::getVertexBuffer()
{
	return m_vertexBuffer;
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