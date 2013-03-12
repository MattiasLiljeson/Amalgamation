#include "RenderInfo.h"

RenderInfo::RenderInfo()
{
	m_instanceId = -1;
	m_meshId = -1;
}

RenderInfo::RenderInfo( int p_mesh_id )
{
	m_meshId = p_mesh_id;
}

RenderInfo::~RenderInfo()
{

}
