// =======================================================================================
//                                      RenderInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # RenderInfo
/// Detailed description.....
/// Created on: 7-12-2012 
///---------------------------------------------------------------------------------------
#pragma once
#include <Component.h>

class RenderInfo : public Component
{
public:
	RenderInfo(bool p_shouldBeRendered=true);
	RenderInfo( int p_mesh_id, bool p_shouldBeRendered=true);
	virtual ~RenderInfo();
	int m_instanceId;
	int m_meshId;
	bool m_shouldBeRendered;
	bool m_shouldBeCulled;
	bool m_shouldBeTesselated;
};