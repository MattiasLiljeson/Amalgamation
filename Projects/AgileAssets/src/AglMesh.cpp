#include "AglMesh.h"

AglMesh::AglMesh(AglMeshHeader p_header, void* p_vertices, unsigned int* p_indices)
{
	m_header = p_header;
	m_vertices = p_vertices;
	m_indices = p_indices;
}
AglMesh::~AglMesh()
{
	if (m_header.vertexFormat == AGL_VERTEX_FORMAT_PNT)
	{
		AglVertexSPNT* v = (AglVertexSPNT*)m_vertices;
		delete[] v;
	}
	else if (m_header.vertexFormat == AGL_VERTEX_FORMAT_PNTTBN)
	{
		AglVertexSTBN* v = (AglVertexSTBN*)m_vertices;
		delete[] v;
	}
	delete[] m_indices;
}
AglMeshHeader AglMesh::getHeader()
{
	return m_header;
}
void* AglMesh::getVertices()
{
	return m_vertices;
}
unsigned int* AglMesh::getIndices()
{
	return m_indices;
}
void AglMesh::setNameIndex(int p_index)
{
	m_header.nameID = p_index;
}