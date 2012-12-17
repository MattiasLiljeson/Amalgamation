#ifndef DEBUGMESH_H
#define DEBUGMESH_H

#include "Utility.h"
#include "AglReader.h"
#include <AglGradient.h>
#include <AglBoundingSphere.h>
#include <AglOBB.h>
#include "VertexStructures.h"

class SkeletonMapping;
class Scene;
struct Material;
class Gradient;

class DebugMesh
{
private:
	//DirectX related
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext;
	ID3D11Buffer*			mVB;
	ID3D11Buffer*			mIB;
	int						mStride;
	unsigned int			mIndexCount;

	AglMesh*	mMesh;

public:
	DebugMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, AglMesh* pMesh);
	~DebugMesh();

	void Init(AglVertexSTBN* pVertices, int pVertexCount, unsigned int* pIndices, int pIndexCount);

	void Draw(AglMatrix pWorld, float pScale = 1.0f);

	AglBoundingSphere	getBoundingSphere();
	AglOBB				getMinimumOBB();
};

#endif