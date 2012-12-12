#include "DebugBox.h"
#include "VertexStructures.h"
#include "ShaderManager.h"
#include "Camera.h"

DebugBox::DebugBox(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;
    VertexPC v[24];
    unsigned long ind[36];

    //Front Face
    v[0] = VertexPC(-AglVector3(0.5f, 0.5f, 0.5f), AglVector3(0, 0, 1),		AglVector4(0, 1, 0, 0));
    v[1] = VertexPC(-AglVector3(0.5f, -0.5f, 0.5f), AglVector3(0, 0, 1),		AglVector4(0, 0, 0, 0));
    v[2] = VertexPC(-AglVector3(-0.5f, -0.5f, 0.5f), AglVector3(0, 0, 1),		AglVector4(1, 0, 0, 0));
    v[3] = VertexPC(-AglVector3(-0.5f, 0.5f, 0.5f), AglVector3(0, 0, 1),		AglVector4(1, 1, 0, 0));

    //Back Face
    v[4] = VertexPC(-AglVector3(0.5f, 0.5f, -0.5f), AglVector3(0, 0, -1),		AglVector4(0, 1, 0, 0));
    v[5] = VertexPC(-AglVector3(-0.5f, 0.5f, -0.5f), AglVector3(0, 0, -1),	AglVector4(0, 0, 0, 0));
    v[6]  = VertexPC(-AglVector3(-0.5f, -0.5f, -0.5f), AglVector3(0, 0, -1),	AglVector4(1, 0, 0, 0));
    v[7] = VertexPC(-AglVector3(0.5f, -0.5f, -0.5f), AglVector3(0, 0, -1),	AglVector4(1, 1, 0, 0));

    //Left Face
    v[8] = VertexPC(-AglVector3(0.5f, 0.5f, -0.5f), AglVector3(-1, 0, 0),		AglVector4(0, 1, 0, 0));
	v[9] = VertexPC(-AglVector3(0.5f, -0.5f, -0.5f), AglVector3(-1, 0, 0),	AglVector4(0, 0, 0, 0));
	v[10] = VertexPC(-AglVector3(0.5f, -0.5f, 0.5f), AglVector3(-1, 0, 0),	AglVector4(1, 0, 0, 0));
    v[11] = VertexPC(-AglVector3(0.5f, 0.5f, 0.5f), AglVector3(-1, 0, 0),		AglVector4(1, 1, 0, 0));

    //Right Face
    v[12] = VertexPC(-AglVector3(-0.5f, 0.5f, -0.5f), AglVector3(1, 0, 0),	AglVector4(0, 1, 0, 0));
    v[13] = VertexPC(-AglVector3(-0.5f, 0.5f, 0.5f), AglVector3(1, 0, 0),		AglVector4(0, 0, 0, 0));
    v[14] = VertexPC(-AglVector3(-0.5f, -0.5f, 0.5f), AglVector3(1, 0, 0),	AglVector4(1, 0, 0, 0));
    v[15] = VertexPC(-AglVector3(-0.5f, -0.5f, -0.5f), AglVector3(1, 0, 0),	AglVector4(1, 1, 0, 0));

    //Top Face
    v[16] = VertexPC(-AglVector3(-0.5f, -0.5f, -0.5f), AglVector3(0, 1, 0),	AglVector4(0, 1, 0, 0));
    v[17] = VertexPC(-AglVector3(-0.5f, -0.5f, 0.5f), AglVector3(0, 1, 0),	AglVector4(0, 0, 0, 0));
    v[18] = VertexPC(-AglVector3(0.5f, -0.5f, 0.5f), AglVector3(0, 1, 0),		AglVector4(1, 0, 0, 0));
    v[19] = VertexPC(-AglVector3(0.5f, -0.5f, -0.5f), AglVector3(0, 1, 0),	AglVector4(1, 1, 0, 0));

    //Bottom Face
    v[20] = VertexPC(-AglVector3(-0.5f, 0.5f, -0.5f), AglVector3(0, -1, 0),	AglVector4(0, 1, 0, 0));
    v[21] = VertexPC(-AglVector3(0.5f, 0.5f, -0.5f), AglVector3(0, -1, 0),	AglVector4(0, 0, 0, 0));
    v[22] = VertexPC(-AglVector3(0.5f, 0.5f, 0.5f), AglVector3(0, -1, 0),		AglVector4(1, 0, 0, 0));
    v[23] = VertexPC(-AglVector3(-0.5f, 0.5f, 0.5f), AglVector3(0, -1, 0),	AglVector4(1, 1, 0, 0));

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


    int curr = 0;
    for (int i = 0; i < 6; i++)
    {
        int b = i * 4;
        ind[curr++] = b;
        ind[curr++] = b+1;
        ind[curr++] = b+2;

        ind[curr++] = b;
        ind[curr++] = b+2;
        ind[curr++] = b+3;
    }
	
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