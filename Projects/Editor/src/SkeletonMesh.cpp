#include "SkeletonMesh.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "SkeletonShader.h"
#include "RasterManager.h"

SkeletonMesh::SkeletonMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, AglSkeleton* pSkeleton)
{
	mDevice = pDevice;
	mDeviceContext = pDeviceContext;
	mSkeleton = pSkeleton;

	AglSkeletonHeader h = mSkeleton->getHeader();
	AglJoint* joints = mSkeleton->getJoints();

	vector<unsigned int> vertices;
	for (int i = 0; i < h.jointCount; i++)
	{
		if (joints[i].parentIndex >= 0) //If the joint is a child
		{
			AglJoint child = joints[i];
			AglJoint parent = joints[joints[i].parentIndex];
			vertices.push_back(child.nodeID);
			vertices.push_back(parent.nodeID);
		}
	}

	//Create Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned int) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	mDevice->CreateBuffer(&bd, &vertexData, &mVB);
	mStride = sizeof(unsigned int);
	mCount = vertices.size();

}
void SkeletonMesh::Draw(AglMatrix pWorld, float pScale)
{
	RasterManager::getInstance()->dsSetNoCull();

	SkeletonShader* ss = ShaderManager::GetInstance()->GetSkeletonShader();
	ss->SetBuffer(pWorld, Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix(), pScale, this);
	mDeviceContext->VSSetShader(ss->GetVertexShader(), 0, 0);
	mDeviceContext->PSSetShader(ss->GetPixelShader(), 0, 0);	
	mDeviceContext->HSSetShader(NULL, 0, 0);
	mDeviceContext->DSSetShader(NULL, 0, 0);
	mDeviceContext->GSSetShader(NULL, 0, 0);
	mDeviceContext->IASetInputLayout(ss->GetInputLayout());

	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &mVB, &mStride, &offset);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	mDeviceContext->Draw(mCount, 0);

	RasterManager::getInstance()->dsSetStandard();
}