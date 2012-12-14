#include "SphereMesh.h"
#include <AglSphereMesh.h>
#include <AglMesh.h>
#include "ShaderManager.h"
#include "Camera.h"
#include "StandardShader.h"
#include "RasterManager.h"

SphereMesh::SphereMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	mDevice = pDevice;
	mDeviceContext = pDeviceContext;

	AglSphereMesh sm;
	vector<AglVertexSTBN> v = vector<AglVertexSTBN>(sm.positions.size());
	for (int i = 0; i < v.size(); i++)
	{
		v[i].position = sm.positions[i];
		v[i].normal = AglVector3(1, 1, 1);//sm.normals[i];
		v[i].texCoord = sm.texCoord[i];
		v[i].tangent = AglVector3(0, 0, 0);
		v[i].binormal = AglVector3(0, 0, 0);
	}

	//Create Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(AglVertexSTBN) * v.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &v[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	mDevice->CreateBuffer(&bd, &vertexData, &mVB);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(unsigned int) * sm.indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &sm.indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	mDevice->CreateBuffer(&ibd, &indexData, &mIB);

	mIndexCount = sm.indices.size();

mStride = sizeof(AglVertexSTBN);
}

SphereMesh::~SphereMesh()
{

}
void SphereMesh::Draw(AglMatrix pWorld)
{
	RasterManager::getInstance()->setWireframeState();
	AglMaterial mat;
	mat.diffuse = AglVector3(100, 0 , 0);
	mat.specular = AglVector3(0, 0, 0);
	StandardShader* ss = ShaderManager::GetInstance()->GetStandardShader();
	ss->SetBuffer(pWorld, Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix(), 1.0f, mat);
	mDeviceContext->VSSetShader(ss->GetVertexShader(), 0, 0);
	mDeviceContext->PSSetShader(ss->GetPixelShader(), 0, 0);	
	mDeviceContext->HSSetShader(ss->GetHullShader(), 0, 0);
	mDeviceContext->DSSetShader(ss->GetDomainShader(), 0, 0);
	mDeviceContext->GSSetShader(NULL, 0, 0);
	mDeviceContext->IASetInputLayout(ss->GetInputLayout());
	UINT stride = mStride;
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	mDeviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	mDeviceContext->DrawIndexed(mIndexCount, 0, 0);
	RasterManager::getInstance()->setStandardState();
}
