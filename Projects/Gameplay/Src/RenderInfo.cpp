#include "RenderInfo.h"

RenderInfo::RenderInfo(bool p_shouldBeRendered/* =true */)
	: Component( ComponentType::RenderInfo )
{
	m_instanceId = -1;
	m_meshId = -1;
	m_shouldBeRendered = p_shouldBeRendered;
	m_shouldBeCulled = false;
}

RenderInfo::RenderInfo(int p_mesh_id, bool p_shouldBeRendered/* =true */)
	: Component( ComponentType::RenderInfo )
{
	m_meshId = p_mesh_id;
	m_shouldBeRendered = p_shouldBeRendered;
	m_shouldBeCulled = false;
}

RenderInfo::~RenderInfo()
{

}
