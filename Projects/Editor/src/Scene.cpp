#include "Scene.h"
#include "Mesh.h"
#include "SkeletonMapping.h"
#include "AglWriter.h"
#include "TextureManager.h"
#include "SkeletonMesh.h"
#include "Globals.h"
#include "SphereMesh.h"
#include "BoxMesh.h"


Scene* Scene::sInstance = NULL;

AglMatrix Scene::m_avoidJump = AglMatrix();
AglMatrix Scene::m_world = AglMatrix();

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
	for (unsigned int i = 0; i < mMeshes.size(); i++)
	{
		delete mMeshes[i];
	}
	for (unsigned int i = 0; i < mSkeletonMeshes.size(); i++)
	{
		delete mSkeletonMeshes[i];
	}	
	for (unsigned int i = 0; i < mSkeletonMappings.size(); i++)
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
		AglSkeleton* s = mAglScene->getSkeleton(0);
		AglJoint* j1 = s->getRoot();

		//What is it?
		m_world = s->getInverseBindMatrix(j1->id);// * s->getGlobalTransform(j1->id);
		m_avoidJump = s->getInverseBindMatrix(j1->id) * s->getGlobalTransform(j1->id);
		//mAglScene->tempFix(m_avoidJump.inverse());

	}
	else
		m_world = AglMatrix::identityMatrix();

	mMeshes = pMeshes;
	mSkeletonMeshes = pSkeletons;
	mSkeletonMappings = pSkeletonMappings;
	mMax = AglVector3(FLT_MIN, FLT_MIN, FLT_MIN);
	mMin = AglVector3(FLT_MAX, FLT_MAX, FLT_MAX);
	for (unsigned int i = 0; i < mMeshes.size(); i++)
	{
		AglVector3 minV = mMeshes[i]->GetMin();
		AglVector3 maxV = mMeshes[i]->GetMax();
		mMax = AglVector3(max(mMax.x, maxV.x), max(mMax.y, maxV.y), max(mMax.z, maxV.z)); 
		mMin = AglVector3(min(mMin.x, minV.x), min(mMin.y, minV.y), min(mMin.z, minV.z)); 

		//Find random colors
		mSphereColors.push_back(RandomUnitVector3());
		mBoxColors.push_back(RandomUnitVector3());
	}
	for (unsigned int i = 0; i < mSkeletonMappings.size(); i++)
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
	for (unsigned int i = 0; i < mMeshes.size(); i++)
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

	if (mAglScene)
	{
		AglMatrix w3 = mAglScene->getCoordinateSystemAsMatrix();
		w = w3 * w;
	}

	if (mAglScene && mAglScene->getSkeletonCount() > 0)
	{
		AglSkeleton* s = mAglScene->getSkeleton(0);
		AglJoint* j1 = s->getRoot();
		m_avoidJump = s->getInverseBindMatrix(j1->id) * s->getGlobalTransform(j1->id);
	}
	else
		m_world = AglMatrix::identityMatrix();

	//AglMatrix::MatrixToComponents(w2, v1, mQuaternionRotation, v2);
	for (unsigned int i = 0; i < mMeshes.size(); i++)
	{
		AglMatrix manip = m_avoidJump.inverse();
		mMeshes[i]->Draw(w, invMax);
		if (SPHEREMESH && DRAWDEBUGSPHERE)
		{
			AglBoundingSphere bs = mMeshes[i]->getBoundingSphere();
			AglMatrix sw;
			AglMatrix::componentsToMatrix(sw, AglVector3(bs.radius, bs.radius, bs.radius), AglQuaternion::identity(), bs.position);
			sw = sw * m_avoidJump;
			sw = sw * invMax;
			sw *= w;
			sw.SetTranslation(sw.GetTranslation() + w.GetTranslation());
			SPHEREMESH->Draw(sw, mSphereColors[i]);
		}
		if (BOXMESH && DRAWDEBUGBOX)
		{
			AglOBB obb = mMeshes[i]->getMinimumOBB();
			AglMatrix sw = obb.world;
			AglMatrix size;
			AglMatrix::componentsToMatrix(size, obb.size, AglQuaternion::identity(), AglVector3(0, 0, 0));
			sw = size * sw;
			sw = sw * m_avoidJump;
			sw = sw * invMax;
			sw *= w;
			sw.SetTranslation(sw.GetTranslation() + w.GetTranslation());
			BOXMESH->Draw(sw, mBoxColors[i]);
		}
	}
	for (unsigned int i = 0; i < mSkeletonMeshes.size(); i++)
		mSkeletonMeshes[i]->Draw(w, invMax);

	AglVector3 minP = mMin;
	AglVector3 maxP = mMax;

	AglMatrix newW = m_world;
	newW.SetTranslation(AglVector3(0, 0, 0));
	minP.transform(newW*invMax);
	maxP.transform(newW*invMax);
	/*if (mPlaneMesh)
	{
		mPlaneMesh->Draw(AglMatrix::createTranslationMatrix(AglVector3(0, min(minP.y, maxP.y), 0)), 1.0f);
	}*/
}
AglNode Scene::GetNode(int pIndex)
{
	return mAglScene->getNode(pIndex);
}

vector<string> Scene::GetMeshNames()
{
	vector<string> names;
	for (unsigned int i = 0; i < mMeshes.size(); i++)
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
	for (unsigned int i = 0; i < materials.size(); i++)
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
		for (unsigned int i = 0; i < mMeshes.size(); i++)
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
		for (unsigned int i = 0; i < mMeshes.size(); i++)
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
bool Scene::IsLeftHanded()
{
	if (!mAglScene)
		return true;
	else
	{
		return mAglScene->isLeftHanded();
	}
}
void Scene::SetCoordinateSystem(AglCoordinateSystem pSystem)
{
	mAglScene->setCoordinateSystem(pSystem);
}
