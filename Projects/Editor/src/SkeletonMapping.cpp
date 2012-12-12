#include "SkeletonMapping.h"
#include "Mesh.h"
#include "Scene.h"

SkeletonMapping::SkeletonMapping(ID3D11Device* pDevice, AglSkeletonMapping* pMapping)//, int pMesh, int pSkeleton, AglSkeletonMappingVertex* pVertices, int pVertexCount)
{
	mAglMapping = pMapping;
	AglSkeletonMappingHeader h = pMapping->getHeader();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(AglSkeletonMappingVertex) * h.vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = pMapping->getVertices();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	mDevice = pDevice;
	mDevice->CreateBuffer(&bd, &vertexData, &mVB);
}
SkeletonMapping::~SkeletonMapping()
{
	mVB->Release();
}
int SkeletonMapping::GetMesh()
{
	return mAglMapping->getHeader().meshID;
}
int SkeletonMapping::GetSkeleton()
{
	return mAglMapping->getHeader().skeletonID;
}
ID3D11Buffer* SkeletonMapping::GetMappingBuffer()
{
	return mVB;
}
int SkeletonMapping::GetStride()
{
	return sizeof(AglSkeletonMappingVertex);
}
int SkeletonMapping::GetVertexCount()
{
	return mAglMapping->getHeader().vertexCount;
}
AglSkeletonMappingVertex* SkeletonMapping::GetVertices()
{
	return mAglMapping->getVertices();
}