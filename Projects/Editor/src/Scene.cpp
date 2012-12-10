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


void Scene::Init(vector<Mesh*> pMeshes, vector<SkeletonMesh*> pSkeletons, vector<SkeletonMapping*> pSkeletonMappings, AglScene* pAglScene, string pFolder)
{
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
	AglVector3 c = (mMax + mMin)*0.25f;
	float invMax = 1.0f / maxV;
	c *= invMax;
	
	AglMatrix w = AglMatrix(cos(mRotation), 0, sin(mRotation), 0, 0, 1, 0, 0, -sin(mRotation), 0, cos(mRotation), 0, -c.x, -c.y, -c.z, 1);
	AglMatrix w2;

	AglVector3 scale(1, 1, 1);
	AglVector3 pos = AglVector3(-c.x, -c.y, -c.z);
	pos += mPosition;
	AglMatrix::componentsToMatrix(w2, scale, mQuaternionRotation, pos);


	//This works but trouble now occurs because of different coordinate systems!
	//Used this before. Linker errors!
	//w2 = AglMatrix::Inverse(m_world) * AglMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1);
	w = AglMatrix(w2[0], w2[1], w2[2], w2[3], w2[4], w2[5], w2[6], w2[7], w2[8], w2[9], w2[10], w2[11], w2[12], w2[13], w2[14], w2[15]);
	
	//Borde inte göras här. Borde göras vid konverteringen från fbx.
	AglMatrix w3(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1);
	w = w3 * w;

	//AglMatrix::MatrixToComponents(w2, v1, mQuaternionRotation, v2);
	for (int i = 0; i < mMeshes.size(); i++)
		mMeshes[i]->Draw(w, invMax);
	for (int i = 0; i < mSkeletonMeshes.size(); i++)
		mSkeletonMeshes[i]->Draw(w, invMax);
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
