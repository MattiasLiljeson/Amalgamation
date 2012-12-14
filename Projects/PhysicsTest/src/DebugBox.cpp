#include "DebugBox.h"
#include "VertexStructures.h"
#include "ShaderManager.h"
#include "Camera.h"
#include <AglBoxMesh.h>

DebugBox::DebugBox(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;

	AglBoxMesh bm;

    VertexPC v[24];
    unsigned long ind[36];

	for (int i = 0; i < 24; i++)
	{
		v[i] = VertexPC(bm.positions[i], bm.normals[i], AglVector4(bm.texCoord[i].x, bm.texCoord[i].y, 0, 0));
	}
	for (int i = 0; i < 36; i++)
	{
		ind[i] = bm.indices[i];
	}

	//Create Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VertexPC) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_device->CreateBuffer(&bd, NULL, &m_vb);

	//Insert Vertices into Vertex Buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	m_deviceContext->Map(m_vb, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, v, sizeof(v));
	m_deviceContext->Unmap(m_vb, NULL); 
	
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(unsigned long) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = ind;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	m_device->CreateBuffer(&ibd, &indexData, &m_ib);
}
void DebugBox::draw(AglMatrix pWorld, bool pWireframe)
{
	if (pWireframe)
		m_deviceContext->RSSetState(DX11Application::mWireframeState);
	SimpleShader* ss = ShaderManager::GetInstance()->GetSimpleShader();
	ss->SetMatrixBuffer(pWorld, Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix(), true);
	m_deviceContext->VSSetShader(ss->GetVertexShader().Data, 0, 0);
	m_deviceContext->PSSetShader(ss->GetPixelShader().Data, 0, 0);	
	m_deviceContext->HSSetShader(NULL, 0, 0);
	m_deviceContext->DSSetShader(NULL, 0, 0);
	m_deviceContext->IASetInputLayout(ss->GetInputLayout());
	UINT stride = sizeof(VertexPC);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vb, &stride, &offset);
	m_deviceContext->IASetIndexBuffer(m_ib, DXGI_FORMAT_R32_UINT, 0);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->DrawIndexed(36, 0, 0);
	m_deviceContext->RSSetState(DX11Application::mRasterState);
}