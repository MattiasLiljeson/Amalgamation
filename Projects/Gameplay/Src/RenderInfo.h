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
	RenderInfo();
	RenderInfo( int p_mesh_id);
	virtual ~RenderInfo();
	int m_instanceId;
	int m_meshId;
private:
};