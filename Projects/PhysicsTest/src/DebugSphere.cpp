#include "DebugSphere.h"
#include "ShaderManager.h"
#include "Camera.h"

DebugSphere::DebugSphere(int pPerimiterPoints, ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;

	vector<VertexPC> v = vector<VertexPC>(pPerimiterPoints * (pPerimiterPoints - 1) + 2);
	float step = (float)(2 * PI / pPerimiterPoints);

	int curr = 0;
	for (int i = 0; i < pPerimiterPoints; i++)
	{
		for (int j = 1; j < pPerimiterPoints; j++)
		{
			float x = (float)cos(step * i);
			float y = 2 * j / (float)pPerimiterPoints - 1.0f;
			float z = (float)sin(step * i);
			float frac = (float)cos(PI * y  / 2);// 1 - y * y;
			AglVector3 pos = AglVector3(x * frac, y, z * frac);
			Normalize(pos);

			float texu = (float)(0.5f * asin(pos[0]) / PI) + 0.5f;
			float texv = (float)(0.5f * asin(pos[1]) / PI) + 0.5f;
			//texu = 0.5f * pos.X + 0.5f;
			//texv = -0.5f * pos.Y + 0.5f;
			AglVector4 c(1, 0, 0, 1);
			if (pos[0] > 0 && pos[1] > 0)
				c = AglVector4(0, 1, 0, 1);
			else if (pos[0] > 0)
				c = AglVector4(1, 1, 0, 1);
			else if (pos[1] > 0)
				c = AglVector4(0, 0, 1, 1);
			v[curr++] = VertexPC(pos, pos, c);
		}
	}
	//v[v.size() - 2] = new VertexPositionNormalTexture(new AglVector3(0, -1, 0), new AglVector3(0, -1, 0), new Vector2(0, 0.0f));
	//v[v.size() - 1] = new VertexPositionNormalTexture(new AglVector3(0, 1, 0), new AglVector3(0, 1, 0), new Vector2(0, 0.0f));
	v[v.size() - 2] = VertexPC(AglVector3(0, -1, 0), AglVector3(0, -1, 0), AglVector4(1, 0, 0, 1));
	v[v.size() - 1] = VertexPC(AglVector3(0, 1, 0), AglVector3(0, 1, 0), AglVector4(1, 0, 0, 1));


	vector<unsigned int> ind = vector<unsigned int>(pPerimiterPoints * (pPerimiterPoints-2)*6 + pPerimiterPoints*3*2);
	curr = 0;
	for (int i = 0; i < pPerimiterPoints; i++)
	{
		for (int j = 0; j < pPerimiterPoints-2; j++)
		{
			int c1 = i * (pPerimiterPoints - 1) + j; 
			int c2 = (i+1) * (pPerimiterPoints - 1) + j;
			ind[curr++] = c1;
			ind[curr++] = c1 + 1;
			ind[curr++] = c2 % (v.size()-2);
			ind[curr++] = c1 + 1;
			ind[curr++] = (c2 + 1) % (v.size()-2);
			ind[curr++] = c2 % (v.size()-2);
		}
	}
	for (int i = 0; i < pPerimiterPoints; i++)
	{
		int c1 = i * (pPerimiterPoints - 1);
		int c2 = ((i + 1) * (pPerimiterPoints - 1)) % (v.size() - 2);
		ind[curr++] = v.size() - 2;
		ind[curr++] = c1;
		ind[curr++] = c2;

		c1 = i * (pPerimiterPoints - 1) + pPerimiterPoints-2;
		c2 = ((i + 1) * (pPerimiterPoints - 1) + pPerimiterPoints-2) % (v.size() - 2);
		ind[curr++] = v.size() - 1;
		ind[curr++] = c2;
		ind[curr++] = c1;
	}
	m_indexCount = ind.size();


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
	ibd.ByteWidth = sizeof(unsigned int) * ind.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &ind[0];
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