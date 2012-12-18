// =======================================================================================
//                                      SphereMesh
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # SphereMesh
/// Detailed description.....
/// Created on: 14-12-2012 
///---------------------------------------------------------------------------------------
#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include "Utility.h"
#include <AglMatrix.h>

class SphereMesh
{
private:
	//DirectX related
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext;
	ID3D11Buffer*			mVB;
	ID3D11Buffer*			mIB;
	int						mStride;

	bool					m_wireframe;
	unsigned int			mIndexCount;
public:
	SphereMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~SphereMesh();
	void Draw(AglMatrix pWorld, AglVector3 pColor);
};

#endif // SPHEREMESH_H