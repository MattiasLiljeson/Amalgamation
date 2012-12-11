#ifndef DEBUGHULL_H
#define DEBUGHULL_H

#include <ConvexHullShape.h>
#include <RigidBodyConvexHull.h>
#include "DebugSphere.h"
#include "DebugBox.h"


// =================================================================================================
//                                         DEBUGHULL
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief DebugHull contains logic for drawing a convex hull shape
///
/// 
/// Edited By: Anton Andersson, 2012-11-14
///-------------------------------------------------------------------------------------------------
class DebugHull
{
private:
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;
	ID3D11Buffer*			m_vb;
	ID3D11Buffer*			m_ib;

	int						m_indices;
public:
	DebugHull(ConvexHullShape*, ID3D11Device*, ID3D11DeviceContext*);
	~DebugHull();
	void draw(AglMatrix pWorld);
};

struct DebugHullData
{
private:
	DebugHull* mDebugHull;
	vector<RigidBodyConvexHull*> mInstances;

public:
	DebugHullData(ConvexHullShape* pShape, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	{
		mDebugHull = new DebugHull(pShape, pDevice, pDeviceContext);
	}
	~DebugHullData();
	void AddInstance(RigidBodyConvexHull* pInstance)
	{
		mInstances.push_back(pInstance);
	}
	void RemoveInstance(RigidBodyConvexHull* pInstance)
	{
		for (unsigned int i = 0; i < mInstances.size(); i++)
		{
			if (pInstance == mInstances[i])
			{
				mInstances[i] = mInstances.back();
				mInstances.pop_back();
			}
		}
	}
	void Draw(DebugSphere* p_s, DebugBox* p_b);
};

#endif