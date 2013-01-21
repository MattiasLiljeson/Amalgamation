#include "RenderInfo.h"

RenderInfo::RenderInfo(bool p_shouldBeRendered/* =true */)
{
	m_instanceId = -1;
	m_meshId = -1;
	m_shouldBeRendered = p_shouldBeRendered;
}

RenderInfo::RenderInfo(int p_mesh_id, bool p_shouldBeRendered/* =true */)
{
	m_meshId = p_mesh_id;
	m_shouldBeRendered = p_shouldBeRendered;
}

RenderInfo::~RenderInfo()
{

}
