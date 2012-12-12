#include "Scene.h"
#include "Mesh.h"
#include "SkeletonMapping.h"
#include "AglWriter.h"
#include "TextureManager.h"
#include "SkeletonMesh.h"

Scene* Scene::sInstance = NULL;

Scene::Scene()
{
	mRotation = 0;
	mCurrentAnimation = 0;
	mAglScene = NULL;
	mQuaternionRotation = AglQuaternion(0, 0, 0, 1);
	mPosition = AglVector3(0, 0, 0);
	mPlaneMesh = NULL;
}
Scene::~Scene()
{
	for (int i = 0; i < mMeshes.size(); i++)
	{
		delete mMeshes[i];
	}
	for (int i = 0; i < mSkeletonMeshes.size(); i++)
	{
		delete mSkeletonMeshes[i];
	}	
	for (int i = 0; i < mSkeletonMappings.size(); i++)
	{
		delete mSkeletonMappings[i];
	}
	if (mAglScene)
		delete mAglScene;
}

Scene* Scene::GetInstance()
{
	if (!sInstance)
		sInstance = new Scene();
	return sInstance;
}
void Scene::Release()
{
	if (sInstance)
		delete sInstance;
	sInstance = NULL;
}


void Scene::Init(vector<Mesh*> pMeshes, vector<SkeletonMesh*> pSkeletons, vector<SkeletonMapping*> pSkeletonMappings, AglScene* pAglScene, string pFolder,
				 ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	mDevice = pDevice;
	mDeviceContext = pDeviceContext;
	mAglScene = pAglScene;
	if (mAglScene->getSkeletonCount() > 0)
	{
		AglJoint* j1 = mAglScene->getSkeleton(0)->getRoot();
		AglNode root = mAglScene->getNode(j1->nodeID);
		m_world = root.localTransform;
	}
	else
		m_world = AglMatrix::identityMatrix();

	mMeshes = pMeshes;
	mSkeletonMeshes = pSkeletons;
	mSkeletonMappings = pSkeletonMappings;
	mMax = AglVector3(FLT_MIN, FLT_MIN, FLT_MIN);
	mMin = AglVector3(FLT_MAX, FLT_MAX, FLT_MAX);
	for (int i = 0; i < mMeshes.size(); i++)
	{
		AglVector3 minV = mMeshes[i]->GetMin();
		AglVector3 maxV = mMeshes[i]->GetMax();
		mMax = AglVector3(max(mMax.x, maxV.x), max(mMax.y, maxV.y), max(mMax.z, maxV.z)); 
		mMin = AglVector3(min(mMin.x, minV.x), min(mMin.y, minV.y), min(mMin.z, minV.z)); 
	}
	for (int i = 0; i < mSkeletonMappings.size(); i++)
	{
		mMeshes[mSkeletonMappings[i]->GetMesh()]->AddSkeletonMapping(mSkeletonMappings[i]);
	}
	AglAnimation* anim = mAglScene->getAnimation(0);
	anim->play();
	mFolder = pFolder;
	CreateScenePlane();
}
void Scene::Update(float pElapsedTime)
{
	for (int i = 0; i < mMeshes.size(); i++)
		mMeshes[i]->update(pElapsedTime);
	if (mAglScene)
		mAglScene->update(pElapsedTime);
	mRotation += pElapsedTime;
}
void Scene::Draw()
{
	float maxV = max(max(mMax.x - mMin.x, mMax.y - mMin.y), mMax.z - mMin.z);
	float invMax = 1.0f / maxV;
	
	AglMatrix w;

	AglVector3 scale(1, 1, 1);
	AglMatrix::componentsToMatrix(w, scale, mQuaternionRotation, mPosition);
	
	//Borde inte göras här. Borde göras vid konverteringen från fbx.
	AglMatrix w3(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1);
	w = w3 * w;

	//AglMatrix::MatrixToComponents(w2, v1, mQuaternionRotation, v2);
	for (int i = 0; i < mMeshes.size(); i++)
		mMeshes[i]->Draw(w, invMax);
	for (int i = 0; i < mSkeletonMeshes.size(); i++)
		mSkeletonMeshes[i]->Draw(w, invMax);


	//Mesh Walker does not work with this solution because of animation
	AglVector3 min = mMin;

	min.transform(m_world);
	if (mPlaneMesh)
		mPlaneMesh->Draw(AglMatrix::createTranslationMatrix(AglVector3(0, min.y*invMax, 0)), 1.0f);
}
AglNode Scene::GetNode(int pIndex)
{
	return mAglScene->getNode(pIndex);
}

vector<string> Scene::GetMeshNames()
{
	vector<string> names;
	for (int i = 0; i < mMeshes.size(); i++)
	{
		stringstream ss;
		ss << i;
		names.push_back("Mesh " + ss.str());
	}
	return names;
}
vector<string> Scene::GetMatrialNames()
{
	vector<string> names;
	vector<AglMaterial*> materials = mAglScene->getMaterials();
	for (int i = 0; i < materials.size(); i++)
	{
		stringstream ss;
		ss << i;
		names.push_back("Material " + ss.str());
	}
	return names;
}
vector<AglMaterial*> Scene::GetMaterials()
{
	return mAglScene->getMaterials();
}
AglMaterial* Scene::GetMaterial(int pIndex)
{
	return mAglScene->getMaterials()[pIndex];
}
void Scene::SetCurrentAnimation(int pIndex)
{
	mCurrentAnimation = pIndex;
}
void Scene::AddMaterial(AglMaterial* pMaterial, bool pAddToMeshes, bool pSetAsCurrent)
{
	mAglScene->addMaterial(pMaterial);
	int count = mAglScene->getMaterials().size();
	if (pAddToMeshes)
	{
		for (int i = 0; i < mMeshes.size(); i++)
		{
			mMeshes[i]->AddMaterial(count-1, pSetAsCurrent);
		}
	}
}
void Scene::AddGradient(AglGradient* pGradient, bool pAddToMeshes, bool pSetAsCurrent)
{
	mAglScene->addGradient(pGradient);
	if (pAddToMeshes)
	{
		for (int i = 0; i < mMeshes.size(); i++)
		{
			mMeshes[i]->AddGradient(pGradient, pSetAsCurrent);
		}
	}
}
vector<AglGradient*> Scene::GetGradients()
{
	if (mAglScene)
		return mAglScene->getGradients();
	return vector<AglGradient*>();
}
Mesh* Scene::GetMesh(int pIndex)
{
	return mMeshes[pIndex];
}
AglSkeleton* Scene::GetSkeleton(int pIndex)
{
	return mAglScene->getSkeleton(pIndex);
}
AglNodeAnimation* Scene::GetNodeAnimation(int pIndex)
{
	return mAglScene->getNodeAnimation(pIndex);
}
AglAnimationLayer* Scene::GetAnimationLayer(int pIndex)
{
	return mAglScene->getAnimationLayer(pIndex);
}
string Scene::GetName(int pIndex)
{
	return mAglScene->getName(pIndex);
}
int Scene::AddName(string pName)
{
	return mAglScene->addName(pName);
}
void Scene::Save(string pPath)
{
	AglWriter w(pPath);
	w.write(mAglScene);
}
AglVector3 Scene::GetCenter() 
{ 
	float maxV = max(max(mMax.x - mMin.x, mMax.y - mMin.y), mMax.z - mMin.z);
	float invMax = 1.0f / maxV*0.5f;
	AglVector3 c = ((mMin+mMax)*0.5f);
	c.transform(m_world*invMax);
	return c;
}
void Scene::CreateScenePlane()
{
	AglVertexSTBN* verts = new AglVertexSTBN[4];
	float size = 2;
	verts[0].position = AglVector3(size, 0, size);
	verts[0].normal	  = AglVector3(0, 1.0f, 0);
	verts[1].position = AglVector3(size, 0, -size);
	verts[1].normal	  = AglVector3(0, 1.0f, 0);
	verts[2].position = AglVector3(-size, 0, size);
	verts[2].normal	  = AglVector3(0, 1.0f, 0);
	verts[3].position = AglVector3(-size, 0, -size);
	verts[3].normal	  = AglVector3(0, 1.0f, 0);

	unsigned int* ind = new unsigned int[6];
	ind[0] = 0;
	ind[1] = 2;
	ind[2] = 1;
	ind[3] = 1;
	ind[4] = 2;
	ind[5] = 3;

	AglMeshHeader h;
	h.id = -1;
	h.indexCount = 6;
	h.vertexCount = 4;
	h.nameID = -1;
	AglMesh* m = new AglMesh(h, verts, ind);
	mPlaneMesh = new Mesh(mDevice, mDeviceContext, this);
	mPlaneMesh->Init(m, NULL);
}
