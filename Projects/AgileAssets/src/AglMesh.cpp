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
void AglMesh::transform(AglMatrix p_transform)
{
	AglVector3 r = p_transform.GetRight();
	AglVector3 u = p_transform.GetUp();
	AglVector3 f = p_transform.GetForward();

	if (AglVector3::dotProduct(AglVector3::crossProduct(r, u), f) < 0)
	{
		//Mirror indices
		for (int i = 0; i < m_header.indexCount; i+=3)
		{
			unsigned int temp = m_indices[i];
			m_indices[i] = m_indices[i+1];
			m_indices[i+1] = temp;
		}
	}

	AglVertexSTBN* v = (AglVertexSTBN*)m_vertices;

	/*for (int i = 0; i < m_header.vertexCount; i++)
	{
		v[i].position.transform(p_transform);
		v[i].normal.transformNormal(p_transform);
		v[i].binormal.transformNormal(p_transform);
		v[i].tangent.transformNormal(p_transform);
	}*/

	m_header.minimumOBB.world *= p_transform;
	m_header.boundingSphere.position.transform(p_transform);
	m_header.transform *= p_transform;

}