#include "DebugMesh.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "SimpleShader.h"

DebugMesh::DebugMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, AglMesh* pMesh)
{
	mDevice = pDevice;
	mDeviceContext = pDeviceContext;

	AglMeshHeader h = pMesh->getHeader();

	AglVertexSTBN* v = (AglVertexSTBN*)pMesh->getVertices();

	unsigned int* ind = pMesh->getIndices();

	Init(v, h.vertexCount, pMesh->getIndices(), h.indexCount);

	mMesh = pMesh;
}
DebugMesh::~DebugMesh()
{
	mVB->Release();
	mIB->Release();
}

void DebugMesh::Draw(AglMatrix pWorld, float pScale)
{
	SimpleShader* ss = ShaderManager::GetInstance()->GetSimpleShader();
	ss->SetMatrixBuffer(pWorld, Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix(), false);
	mDeviceContext->VSSetShader(ss->GetVertexShader().Data, 0, 0);
	mDeviceContext->PSSetShader(ss->GetPixelShader().Data, 0, 0);	
	mDeviceContext->HSSetShader(NULL, 0, 0);
	mDeviceContext->DSSetShader(NULL, 0, 0);
	mDeviceContext->IASetInputLayout(ss->GetInputLayout());
	UINT stride = sizeof(VertexPC);
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	mDeviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mDeviceContext->DrawIndexed(mIndexCount, 0, 0);
}
AglBoundingSphere DebugMesh::getBoundingSphere()
{
	AglMeshHeader mh = mMesh->getHeader();
	return mh.boundingSphere;
}
AglOBB DebugMesh::getMinimumOBB()
{
	AglMeshHeader mh = mMesh->getHeader();
	return mh.minimumOBB;
}
void DebugMesh::Init(AglVertexSTBN* pVertices, int pVertexCount, unsigned int* pIndices, int pIndexCount)
{
	VertexPC* v = new VertexPC[pVertexCount];
	for (int i = 0; i < pVertexCount; i++)
	{
		v[i].position = pVertices[i].position;
		v[i].normal = pVertices[i].normal;
		v[i].color = AglVector4(1, 0, 0, 1);
	}

	//Create Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VertexPC) * pVertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = v;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	mDevice->CreateBuffer(&bd, &vertexData, &mVB);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(unsigned int) * pIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = pIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	mDevice->CreateBuffer(&ibd, &indexData, &mIB);

	mStride = sizeof(VertexPC);
	mIndexCount = pIndexCount;
}