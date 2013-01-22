#pragma once

#include <Component.h>
#include <AglOBB.h>
#include <AglBoundingSphere.h>
#include <AglMatrix.h>

// =======================================================================================
//                                      MeshInitData
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # BoundingVolumeInitData
/// Detailed description.....
/// Created on: 22-1-2013 
///---------------------------------------------------------------------------------------

class BoundingVolumeInitData : public Component
{
public:
	BoundingVolumeInitData(const AglBoundingSphere& p_boundingSphere, const AglOBB& p_obb,
				AglMatrix p_worldTransform)
	{
		m_boundingSphere	= p_boundingSphere;
		m_obb				= p_obb;
		m_worldMatrix		= p_worldTransform;
	}

	const AglMatrix&			getWorldTransform();
	const AglBoundingSphere&	getBoundingSphere();
	const AglOBB&				getOBB();

protected:
private:
	AglBoundingSphere	m_boundingSphere;	// Bounding sphere in WORLD space.
	AglOBB				m_obb;				// Bounding box in WORLD space.
	AglMatrix			m_worldMatrix;
};