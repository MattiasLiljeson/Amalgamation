#include "DebugHull.h"
#include "VertexStructures.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "VertexStructures.h"

DebugHull::DebugHull(ConvexHullShape* p_shape, ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;

	vector<unsigned int> shapeInd = p_shape->GetIndices();
	vector<AglVector3> shapeVert = p_shape->GetVertices();

	VertexPC* v = new VertexPC[shapeInd.size()];
	unsigned long* ind = new unsigned long[shapeInd.size()];
	//Vector4 c(1, 0, 0, 1);
	for (unsigned int i = 0; i < shapeInd.size(); i+=3)
	{
		AglVector4 c(RandomFloat(0, 1), RandomFloat(0, 1), RandomFloat(0, 1), 1);
		ind[i] = i;
		ind[i+1] = i+1;
		ind[i+2] = i+2;
		AglVector3 v1 = shapeVert[shapeInd[i]];
		AglVector3 v2 = shapeVert[shapeInd[i+1]];
		AglVector3 v3 = shapeVert[shapeInd[i+2]];
		AglVector3 norm = AglVector3::crossProduct(v2-v1, v3-v1);
		Normalize(norm);
		v[i] = VertexPC(shapeVert[shapeInd[i]], norm, c);
		v[i+1] = VertexPC(shapeVert[shapeInd[i+1]], norm, c);
		v[i+2] = VertexPC(shapeVert[shapeInd[i+2]], norm, c);
	}

	m_indices = shapeInd.size();

	//Create Vertex Buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VertexPC) * shapeInd.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = v;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	m_device->CreateBuffer(&bd, &vertexData, &m_vb);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(unsigned long) * shapeInd.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = ind;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	m_device->CreateBuffer(&ibd, &indexData, &m_ib);
	delete[] v;
	delete[] ind;
}
DebugHull::~DebugHull()
{
	m_vb->Release();
	m_ib->Release();
}
void DebugHull::draw(AglMatrix pWorld)
{
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

	m_deviceContext->DrawIndexed(m_indices, 0, 0);
}






DebugHullData::~DebugHullData()
{
	delete mDebugHull;
}

void DebugHullData::Draw(DebugSphere* p_s, DebugBox* p_b)
{
	for (unsigned int i = 0; i < mInstances.size(); i++)
	{
		AglMatrix m = mInstances[i]->GetWorld();
		AglMatrix world = AglMatrix(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9],
			m[10], m[11], m[12], m[13], m[14], m[15]);
		m = mInstances[i]->GetSizeAsMatrix();
		AglMatrix size = AglMatrix(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9],
			m[10], m[11], m[12], m[13], m[14], m[15]);
		mDebugHull->draw(size * world);

		if (false)//p_s)
		{
			if (!mInstances[i]->IsStatic())
			{
				float radius = mInstances[i]->GetBoundingSphere().radius;
				world = AglMatrix(radius, 0, 0, 0, 0, radius, 0, 0, 0, 0, radius, 0, 0, 0, 0, 1) * world;
				p_s->draw(world, true);
			}
		}
		if (p_b)
		{
			if (!mInstances[i]->IsStatic())
			{
				float hs = mInstances[i]->GetSize();
				m = mInstances[i]->GetOBB().world;
				m[12] = m[12]*hs;
				m[13] = m[13]*hs;
				m[14] = m[14]*hs;
				m *= mInstances[i]->GetWorld();
				AglMatrix world = AglMatrix(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9],
					m[10], m[11], m[12], m[13], m[14], m[15]);

				AglVector3 s = mInstances[i]->GetOBB().size;
				AglMatrix size = AglMatrix(s.x*hs, 0, 0, 0, 0, s.y*hs, 0, 0, 0, 0,
					s.z*hs, 0, 0, 0, 0, 1);
				p_b->draw((AglMatrix)(size * world), true);
			}
		}
	}
}