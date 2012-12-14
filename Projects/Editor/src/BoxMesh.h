// =======================================================================================
//                                      BoxMesh
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # BoxMesh
/// Detailed description.....
/// Created on: 14-12-2012 
///---------------------------------------------------------------------------------------
#ifndef BOXMESH_H
#define BOXMESH_H

#include "Utility.h"
#include <AglMatrix.h>

class BoxMesh
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
	BoxMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~BoxMesh();
	void Draw(AglMatrix pWorld);
};

#endif // BOXMESH_H