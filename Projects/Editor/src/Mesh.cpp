#include "Mesh.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "AGLLoader.h"
#include "Scene.h"
#include "SkeletonMapping.h"
#include "SkeletonMeshShader.h"
#include "StandardShader.h"
#include "GradientShader.h"
#include "RasterManager.h"
#include "RenderNormalsShader.h"
#include "AglLooseBspTree.h"
#include "Globals.h"
#include "SphereMesh.h"
#include "AglOBB.h"
#include "BoxMesh.h"

Mesh::Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, Scene* pScene)
{
	mDevice = pDevice;
	mDeviceContext = pDeviceContext;
	mScene = pScene;
	mMesh = NULL;
	m_wireframe = false;
	m_normalLength = 0;
	spos = AglVector3(0, 0, 0);
	bsptree = NULL;
	m_grid = NULL;
	mCurrentMaterial = -1;
	overrideMaterial = NULL;
}
Mesh::~Mesh()
{
	mVB->Release();
	mIB->Release();
}

void Mesh::Init(AglMesh* pMesh)
{
	AglMeshHeader h = pMesh->getHeader();

	AglVertexSTBN* v = (AglVertexSTBN*)pMesh->getVertices();

	unsigned int* ind = pMesh->getIndices();

	Init<AglVertexSTBN>(v, h.vertexCount, pMesh->getIndices(), h.indexCount);

	AglVector3 minP, maxP;
	minP = maxP = AglVector3(v[0].position.x, v[0].position.y, v[0].position.z);
	for (int i = 1; i < h.vertexCount; i++)
	{
		AglVector3 p = v[i].position;
		p.transform(h.transform);
		minP = AglVector3(min(minP.x, p.x), min(minP.y, p.y), min(minP.z, p.z));
		maxP = AglVector3(max(maxP.x, p.x), max(maxP.y, p.y), max(maxP.z, p.z));
	}
	mMin = minP;
	mMax = maxP;
	
	mVisible = true;
	mCurrentMaterial = 0;
	mMesh = pMesh;
}

AglVector3 Mesh::GetMin()
{
	return mMin;
}
AglVector3 Mesh::GetMax()
{
	return mMax;
}

void Mesh::Draw(AglMatrix pWorld, float pScale)
{
	if (mSkeletonMappings.size() <= 0)
	{
		AglMatrix trans = mMesh->getHeader().transform;
		trans.SetTranslation(trans.GetTranslation()*pScale);
		pWorld = trans * pWorld;
	}
	if (m_grid && m_drawSphereGrid)
	{
		AglInteriorSphereGridHeader gh = m_grid->getHeader();
		AglInteriorSphere* spheres = m_grid->getSpheres();
		for (unsigned int i = 0; i < gh.sphereCount; i++)
		{
			AglVector3 s(spheres[i].radius, spheres[i].radius, spheres[i].radius);
			s *= pScale;
			AglMatrix mat;
			AglMatrix::componentsToMatrix(mat, s, AglQuaternion::identity(), spheres[i].center * pScale);
			mat = mat * pWorld;
			SPHEREMESH->Draw(mat, AglVector3(0, 1, 1));
		}
	}
	if (bsptree && m_drawTree)
	{

		AglLooseBspTreeHeader h = bsptree->getHeader();
		unsigned int start = 0;
		unsigned int stop;
		for (unsigned int i = 0; i < m_treeLevel; i++)
		{
			start = start * 2 + 1;
		}
		stop = start * 2 + 1;
		if (stop > h.nodeCount)
		{
			start = 0;
			stop = 1;
			m_treeLevel = 0;
		}

		AglBspNode* nodes = bsptree->getNodes();
		for (unsigned int i = start; i < stop; i++)
		{
			AglVector3 c = (nodes[i].maxPoint + nodes[i].minPoint) * 0.5f;
			AglVector3 s = (nodes[i].maxPoint - nodes[i].minPoint);
			s *= pScale;
			AglMatrix mat;
			AglMatrix::componentsToMatrix(mat, s, AglQuaternion::identity(), c * pScale);
			mat = mat * pWorld;
			BOXMESH->Draw(mat, AglVector3(1, 1, 0));
		}
	}

	if (m_normalLength > 0)
		DrawNormals(pWorld, pScale);
	if (m_wireframe)
		RasterManager::getInstance()->setWireframeState();
	else if (Scene::GetInstance()->IsLeftHanded())
		RasterManager::getInstance()->setStandardState();
	else
		RasterManager::getInstance()->setInvertedState();
	vector<AglMaterial*> materials = Scene::GetInstance()->GetMaterials();
	AglMaterial mat;
	AglMaterial matp;
	if (materials.size() > 0 && mCurrentMaterial >= 0)
	{
		matp = *Scene::GetInstance()->GetMaterial(mCurrentMaterial);
	}
	else
		matp = mat;

	if (overrideMaterial)
		matp = *overrideMaterial;
	if (mVisible)
	{
		if (mGradients.size() > 0)
		{
				GradientShader* gs = ShaderManager::GetInstance()->GetGradientShader();
				gs->SetBuffer(pWorld, Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix(), pScale, mGradients[0]);
				mDeviceContext->VSSetShader(gs->GetVertexShader(), 0, 0);
				mDeviceContext->PSSetShader(gs->GetPixelShader(), 0, 0);	
				mDeviceContext->HSSetShader(NULL, 0, 0);
				mDeviceContext->DSSetShader(NULL, 0, 0);
				mDeviceContext->GSSetShader(NULL, 0, 0);
				mDeviceContext->IASetInputLayout(gs->GetInputLayout());
				UINT stride = mStride;
				UINT offset = 0;
				mDeviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
				mDeviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
				mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				mDeviceContext->DrawIndexed(mMesh->getHeader().indexCount, 0, 0);
		}
		else
		{
			if (mSkeletonMappings.size() > 0)
			{
				SkeletonMeshShader* ss = ShaderManager::GetInstance()->GetSkeletonMeshShader();
				ss->SetBuffer(pWorld, Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix(), pScale, Scene::GetInstance()->GetSkeleton(mSkeletonMappings[0]->GetSkeleton()), matp);
				mDeviceContext->VSSetShader(ss->GetVertexShader(), 0, 0);
				mDeviceContext->PSSetShader(ss->GetPixelShader(), 0, 0);	
				mDeviceContext->HSSetShader(NULL, 0, 0);
				mDeviceContext->DSSetShader(NULL, 0, 0);
				mDeviceContext->GSSetShader(NULL, 0, 0);
				mDeviceContext->IASetInputLayout(ss->GetInputLayout());

				ID3D11Buffer* vb[2];
				vb[0] = mVB;
				vb[1] = mSkeletonMappings[0]->GetMappingBuffer();
				UINT strides[2];
				strides[0] = mStride;
				strides[1] = mSkeletonMappings[0]->GetStride();
				UINT offsets[2] = {0,0};
				mDeviceContext->IASetVertexBuffers(0, 2, vb, strides, offsets);
				mDeviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
				mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				mDeviceContext->DrawIndexed(mMesh->getHeader().indexCount, 0, 0);
			}
			else
			{
				StandardShader* ss = ShaderManager::GetInstance()->GetStandardShader();
				ss->SetBuffer(pWorld, Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix(), pScale, matp);
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
				mDeviceContext->DrawIndexed(mMesh->getHeader().indexCount, 0, 0);
			}
		}
	}
}
void Mesh::DrawNormals(AglMatrix pWorld, float pScale)
{
	if (mSkeletonMappings.size() > 0)
	{
		RenderNormalsShader* ss = ShaderManager::GetInstance()->GetRenderNormals();
		ss->SetBuffer(pWorld, Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix(), pScale, Scene::GetInstance()->GetSkeleton(mSkeletonMappings[0]->GetSkeleton()), m_normalLength);
		mDeviceContext->VSSetShader(ss->GetVertexShader(), 0, 0);
		mDeviceContext->PSSetShader(ss->GetPixelShader(), 0, 0);	
		mDeviceContext->HSSetShader(NULL, 0, 0);
		mDeviceContext->DSSetShader(NULL, 0, 0);
		mDeviceContext->GSSetShader(ss->GetGeometryShader(), 0, 0);
		mDeviceContext->IASetInputLayout(ss->GetInputLayout());

		ID3D11Buffer* vb[2];
		vb[0] = mVB;
		vb[1] = mSkeletonMappings[0]->GetMappingBuffer();
		UINT strides[2];
		strides[0] = mStride;
		strides[1] = mSkeletonMappings[0]->GetStride();
		UINT offsets[2] = {0,0};
		mDeviceContext->IASetVertexBuffers(0, 2, vb, strides, offsets);
		mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		mDeviceContext->Draw(mMesh->getHeader().vertexCount, 0);
	}
	else
	{
		RenderNormalsShader* ss = ShaderManager::GetInstance()->GetRenderNormals();
		ss->SetBuffer2(pWorld, Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix(), pScale, m_normalLength);
		mDeviceContext->VSSetShader(ss->GetVertexShader2(), 0, 0);
		mDeviceContext->PSSetShader(ss->GetPixelShader(), 0, 0);	
		mDeviceContext->HSSetShader(NULL, 0, 0);
		mDeviceContext->DSSetShader(NULL, 0, 0);
		mDeviceContext->GSSetShader(ss->GetGeometryShader(), 0, 0);
		mDeviceContext->IASetInputLayout(ss->GetInputLayout2());

		UINT stride = mStride;
		UINT offsets = 0;
		mDeviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &stride);
		mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		mDeviceContext->Draw(mMesh->getHeader().vertexCount, 0);
	}
}
void Mesh::AddSkeletonMapping(SkeletonMapping* pSkeletonMapping)
{
	mSkeletonMappings.push_back(pSkeletonMapping);
}
void Mesh::AddMaterial(int pMaterial, bool pSetAsCurrent)
{
	//Not defined yet!
	/*for (int i = 0; i < mMaterials.size(); i++)
	{
		if (mMaterials[i] == pMaterial)
		{
			if (pSetAsCurrent)
				mCurrentMaterial = i;
			return;
		}
	}
	mMaterials.push_back(pMaterial);
	if (pSetAsCurrent)
		mCurrentMaterial = mMaterials.size() - 1;*/
}
void Mesh::AddGradient(AglGradient* pGradient, bool pSetAsCurrent)
{
	for (unsigned int i = 0; i < mGradients.size(); i++)
	{
		if (mGradients[i] == pGradient)
		{
			return;
		}
	}
	mGradients.push_back(pGradient);
}
int Mesh::GetIndexCount()
{
	return mMesh->getHeader().indexCount;
}
int Mesh::GetVertexCount()
{
	return mMesh->getHeader().vertexCount;
}
string Mesh::GetName()
{ 
	return mScene->GetName(mMesh->getHeader().nameID); 
}
void Mesh::SetName(string pName)
{
	mMesh->setNameIndex(mScene->AddName(pName));
}
void Mesh::update(float p_elapsedTime)
{
	spos = AglVector3(0, 0, 0.0f);
	//spos = AglVector3(0, 0, 0.49f);
	if (m_grid)
	{
		if (m_grid->evaluate(spos, 0.01f))
			return;		
	}
	if (bsptree)
	{
		for (int i = 0; i < 50; i++)
			bsptree->evaluate(spos, 0.01f);
	}
}
AglBoundingSphere Mesh::getBoundingSphere()
{
	AglMeshHeader mh = mMesh->getHeader();
	return mh.boundingSphere;
}
AglOBB Mesh::getMinimumOBB()
{
	AglMeshHeader mh = mMesh->getHeader();
	return mh.minimumOBB;
}
void Mesh::createSphereGrid()
{
	if (!m_grid)
	{
		AglMeshHeader h = mMesh->getHeader();
		AglVertexSTBN* v = (AglVertexSTBN*)mMesh->getVertices();
		unsigned int* ind = mMesh->getIndices();
	
		vector<AglVector3> vertices;
		vector<unsigned int> indices;
		for (int i = 0; i < h.vertexCount; i++)
		{
			vertices.push_back(v[i].position);
		}
		for (int i = 0; i < h.indexCount; i++)
		{
			indices.push_back(ind[i]);
		}
		m_grid = new AglInteriorSphereGrid(3, vertices, indices, h.id);
	}
}
void Mesh::createBspTree()
{
	if (!bsptree)
	{
		AglMeshHeader h = mMesh->getHeader();
		AglVertexSTBN* v = (AglVertexSTBN*)mMesh->getVertices();
		unsigned int* ind = mMesh->getIndices();

		vector<AglVector3> vertices;
		vector<unsigned int> indices;
		for (int i = 0; i < h.vertexCount; i++)
		{
			vertices.push_back(v[i].position);
		}
		for (int i = 0; i < h.indexCount; i++)
		{
			indices.push_back(ind[i]);
		}
		AglLooseBspTreeConstructor constructor(h.id, vertices, indices);
		bsptree = constructor.createTree();
		mScene->getAglScene()->addBspTree(bsptree);
	}
}