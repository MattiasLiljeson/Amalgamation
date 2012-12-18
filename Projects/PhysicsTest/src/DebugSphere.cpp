#include "DebugSphere.h"
#include "ShaderManager.h"
#include "Camera.h"
#include <AglSphereMesh.h>

DebugSphere::DebugSphere(int pPerimiterPoints, ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;

	AglSphereMesh sm;
	vector<VertexPC> v = vector<VertexPC>(sm.positions.size());
	for (int i = 0; i < v.size(); i++)
	{
		if (sm.positions[i].x > 0)
			v[i] = VertexPC(sm.positions[i], sm.normals[i], AglVector4(0, 0, 1, 1));
		else
			v[i] = VertexPC(sm.positions[i], sm.normals[i], AglVector4(1, 1, 0, 1));
	}

	m_indexCount = sm.indices.size();


	//Create Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VertexPC) * v.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &v[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	m_device->CreateBuffer(&bd, &vertexData, &m_vb);
	
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

	m_device->CreateBuffer(&ibd, &indexData, &m_ib);
}

void DebugSphere::draw(AglMatrix pWorld, bool pWireframe)
{
	if (pWireframe)
		m_deviceContext->RSSetState(DX11Application::mWireframeState);
	SimpleShader* ss = ShaderManager::GetInstance()->GetSimpleShader();
	ss->SetMatrixBuffer(pWorld, Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix(), false);
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

	m_deviceContext->DrawIndexed(m_indexCount, 0, 0);
	m_deviceContext->RSSetState(DX11Application::mRasterState);
}