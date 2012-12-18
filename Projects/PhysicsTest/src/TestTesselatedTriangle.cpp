#include "TestTesselatedTriangle.h"
#include "ShaderManager.h"
#include "Camera.h"

TestTesselatedTriangle::TestTesselatedTriangle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	mDevice = pDevice;
	mDeviceContext = pDeviceContext;
	InitVB();
}
TestTesselatedTriangle::~TestTesselatedTriangle()
{
	mVB->Release();
}
void TestTesselatedTriangle::InitVB()
{
	//Create Vertices
	VertexPC vertices[] =
	{
		VertexPC(AglVector3(0.0f, 0.5f, 0.0f), AglVector3(0, 0, -1),AglVector4(1.0f, 0.0f, 0.0f, 1.0f)),
		VertexPC(AglVector3(0.45f, -0.5, 0.0f), AglVector3(0, 0, -1),AglVector4(0.0f, 1.0f, 0.0f, 1.0f)),
		VertexPC(AglVector3(-0.45f, -0.5f, 0.0f), AglVector3(0, 0, -1),AglVector4(0.0f, 0.0f, 1.0f, 1.0f))
	};

	AglVector3 v1 = AglVector3(0.45f, -0.5, 0.0f) - AglVector3(0.0f, 0.5f, 0.0f);
	AglVector3 v2 = AglVector3(-0.45f, -0.5f, 0.0f) - AglVector3(0.45f, -0.5, 0.0f);
	AglVector3 n = AglVector3::crossProduct(v1, v2);
	Normalize(n);
	float d = -AglVector3::dotProduct(n, AglVector3(0.0f, 0.5f, 0.0f));
	float dist = AglVector3::dotProduct(n, Camera::GetInstance()->GetPosition()) + d;


	//Create Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VertexPC) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mDevice->CreateBuffer(&bd, NULL, &mVB);

	//Insert Vertices into Vertex Buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	mDeviceContext->Map(mVB, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, sizeof(vertices));
	mDeviceContext->Unmap(mVB, NULL);    


	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(unsigned long) * 3;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;


	unsigned long indices[] = {0, 1, 2};
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	mDevice->CreateBuffer(&ibd, &indexData, &mIB);
}
void TestTesselatedTriangle::Draw()
{
	SimpleTessShader* ss = ShaderManager::GetInstance()->GetSimpleTessShader();
	AglMatrix id(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	ss->SetMatrixBuffer(id, Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix());
	mDeviceContext->VSSetShader(ss->GetVertexShader().Data, 0, 0);
	mDeviceContext->PSSetShader(ss->GetPixelShader().Data, 0, 0);
	mDeviceContext->HSSetShader(ss->GetHullShader().Data, 0, 0);
	mDeviceContext->DSSetShader(ss->GetDomainShader().Data, 0, 0);
	mDeviceContext->IASetInputLayout(ss->GetInputLayout());
	UINT stride = sizeof(VertexPC);
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	mDeviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	mDeviceContext->DrawIndexed(3, 0, 0);
}