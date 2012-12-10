#include "Mesh.h"

Mesh::Mesh(Buffer<PTNVertex>* p_mesh , Buffer<DIndex>* p_indices)
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

Buffer<PTNVertex>* Mesh::getVertexBuffer()
{
	return m_vertexBuffer;
}

Buffer<DIndex>* Mesh::getIndexBuffer()
{
	return m_indexBuffer;
}

unsigned int Mesh::getTextureId()
{
	return m_textureId;
}

void Mesh::setTextureId( unsigned int p_id )
{
	m_textureId = p_id;
}

