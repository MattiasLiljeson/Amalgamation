#include "Mesh.h"

Mesh::Mesh(Buffer<PTNVertex>* p_mesh , Buffer<DIndex>* p_indices)
{
	m_mesh		= p_mesh;
	m_indices	= p_indices;
}

Mesh::~Mesh()
{
	/************************************************************************/
	/* THERE SHOULDN'T BE AN DELETION HERE IN THE FINAL VERSION				*/
	/************************************************************************/
	delete m_mesh;
	delete m_indices;
}

Buffer<PTNVertex>* Mesh::getMesh()
{
	return m_mesh;
}

Buffer<DIndex>* Mesh::getIndicies()
{
	return m_indices;
}

unsigned int Mesh::getInstanceId( unsigned int p_idx )
{
	return m_instanceList[p_idx];
}

void Mesh::addInstanceId( unsigned int p_instanceId )
{
	m_instanceList.add(p_instanceId);
}

void Mesh::clearInstanceList( unsigned int p_idx )
{
	m_instanceList.clear();
}
