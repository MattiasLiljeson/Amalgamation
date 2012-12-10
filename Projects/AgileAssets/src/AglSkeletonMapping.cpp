#include "AglSkeletonMapping.h"

AglSkeletonMapping::AglSkeletonMapping()
{
	m_header.mappingType = m_header.meshID = m_header.skeletonID = -1;
	m_header.vertexCount = 0;
	m_vertices = 0;
}
AglSkeletonMapping::AglSkeletonMapping(AglSkeletonMappingHeader p_header, AglSkeletonMappingVertex* p_vertices)
{
	m_header = p_header;
	m_vertices = p_vertices;
}
AglSkeletonMapping::~AglSkeletonMapping()
{
	delete[] m_vertices;
}

AglSkeletonMappingHeader AglSkeletonMapping::getHeader()
{
	return m_header;
}
AglSkeletonMappingVertex* AglSkeletonMapping::getVertices()
{
	return m_vertices;
}