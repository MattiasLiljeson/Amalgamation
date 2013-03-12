#ifndef SKELETONMESH_H
#define SKELETONMESH_H

#include "Utility.h"
#include <AglSkeleton.h>

class SkeletonMesh
{
private:
	ID3D11Buffer* mVB;
	AglSkeleton* mSkeleton;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	unsigned int mStride;
	unsigned int mCount;
public:
	SkeletonMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, AglSkeleton* pSkeleton);
	AglSkeleton* GetSkeleton(){ return mSkeleton; }
	void Draw(AglMatrix pWorld, float pScale);
};

#endif