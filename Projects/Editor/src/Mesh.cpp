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
}
Mesh::~Mesh()
{
	mVB->Release();
	mIB->Release();
}

void Mesh::Init(AglMesh* pMesh, AglReader* pReader)
{
	AglMeshHeader h = pMesh->getHeader();

	AglVertexSTBN* v = (AglVertexSTBN*)pMesh->getVertices();

	unsigned int* ind = pMesh->getIndices();

	Init<AglVertexSTBN>(v, h.vertexCount, pMesh->getIndices(), h.indexCount);

	AglVector3 minP, maxP;
	minP = maxP = AglVector3(v[0].position.x, v[0].position.y, v[0].position.z);
	for (int i = 1; i < h.vertexCount; i++)
	{
		minP = AglVector3(min(minP.x, v[i].position.x), min(minP.y, v[i].position.y), min(minP.z, v[i].position.z));
		maxP = AglVector3(max(maxP.x, v[i].position.x), max(maxP.y, v[i].position.y), max(maxP.z, v[i].position.z));
	}
	mMin = minP;
	mMax = maxP;
	
	mVisible = true;
	mCurrentMaterial = 0;
	mMesh = pMesh;
}

AglVector3 Mesh::GetMin()
{
	/*AglMatrix w = mScene->GetWorld();
	Matrix world(w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9], w[10], w[11], w[12], w[13], w[14], w[15]);
	AglVector3 minP, maxP;
	AglVertexSTBN* v = (AglVertexSTBN*)mMesh->getVertices();
	AglVector3 p = AglVector3(v[0].position[0], v[0].position[1], v[0].position[2]);
	Vec3Transform(p, world);
	minP = maxP = p;
	for (int i = 1; i < mMesh->GetHeader().VertexCount; i++)
	{
		AglVector3 p = AglVector3(v[i].Position[0], v[i].Position[1], v[i].Position[2]);
		Vec3Transform(p, world);
		minP = AglVector3(min(minP.x, p.x), min(minP.y, p.y), min(minP.z, p.z));
		maxP = AglVector3(max(maxP.x, p.x), max(maxP.y, p.y), max(maxP.z, p.z));
	}
	mMin = minP;
	mMax = maxP;*/
	return mMin;
}
AglVector3 Mesh::GetMax()
{
	/*AglMatrix w = mScene->GetWorld();
	Matrix world(w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9], w[10], w[11], w[12], w[13], w[14], w[15]);
	AglVector3 minP, maxP;
	AglVertexSTBN* v = (AglVertexSTBN*)mMesh->GetVertices();
	AglVector3 p = AglVector3(v[0].Position[0], v[0].Position[1], v[0].Position[2]);
	Vec3Transform(p, world);
	minP = maxP = p;
	for (int i = 1; i < mMesh->GetHeader().VertexCount; i++)
	{
		AglVector3 p = AglVector3(v[i].Position[0], v[i].Position[1], v[i].Position[2]);
		Vec3Transform(p, world);
		minP = AglVector3(min(minP.x, p.x), min(minP.y, p.y), min(minP.z, p.z));
		maxP = AglVector3(max(maxP.x, p.x), max(maxP.y, p.y), max(maxP.z, p.z));
	}
	mMin = minP;
	mMax = maxP;*/
	return mMax;
}

void Mesh::Draw(AglMatrix pWorld)
{
	Draw(pWorld, 1.0f);
}
void Mesh::Draw(AglMatrix pWorld, float pScale)
{
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
	if (materials.size() > 0)
	{
		matp = *Scene::GetInstance()->GetMaterial(mCurrentMaterial);
	}
	else
		matp = mat;
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
			if (false)//mSkeletonMappings.size() > 0)
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