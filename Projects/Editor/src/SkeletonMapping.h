#ifndef SKELETONMAPPING_H
#define SKELETONMAPPING_H

#include "AglSkeletonMapping.h"
#include "Utility.h"

class Skeleton;
class Mesh;

class SkeletonMapping
{
private:
	ID3D11Device*			mDevice;
	ID3D11Buffer*			mVB;
	AglSkeletonMapping*		mAglMapping;

public:
	SkeletonMapping(ID3D11Device* pDevice, AglSkeletonMapping* pMapping);//int pMesh, int pSkeleton, AglSkeletonMappingVertex* pVertices, int pVertexCount);
	~SkeletonMapping();
	int GetMesh();
	int GetSkeleton();
	ID3D11Buffer* GetMappingBuffer();
	int GetStride();
	int GetVertexCount();
	AglSkeletonMappingVertex* GetVertices();
};

#endif