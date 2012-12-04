#include "Box.h"

Box::Box(Buffer<PTNVertex>* p_mesh , Buffer<DIndex>* p_indices)
{
	m_mesh		= p_mesh;
	m_indices	= p_indices;
}

Box::~Box()
{
	/************************************************************************/
	/* THERE SHOULDN'T BE AN DELETION HERE IN THE FINAL VERSION				*/
	/************************************************************************/
	delete m_mesh;
	delete m_indices;
}

Buffer<PTNVertex>* Box::getMesh()
{
	return m_mesh;
}

Buffer<DIndex>* Box::getIndicies()
{
	return m_indices;
}
