#include "Scene.h"
#include "Mesh.h"
#include "SkeletonMapping.h"
#include "AglWriter.h"
#include "TextureManager.h"
#include "SkeletonMesh.h"
#include "Globals.h"
#include "SphereMesh.h"
#include "BoxMesh.h"
#include "ParticleSystem.h"
#include "Camera.h"


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
	for (unsigned int i = 0; i < mParticleSystems.size(); i++)
	{
		delete mParticleSystems[i];
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
void Scene::Init(string pPath, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	AglReader r(pPath.c_str());
	mAglScene = r.getScene();
	mDevice = pDevice;
	mDeviceContext = pDeviceContext;

	vector<AglMesh*> meshes = mAglScene->getMeshes();
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		Mesh* m = new Mesh(mDevice, mDeviceContext, Scene::GetInstance());
		m->Init(meshes[i]);
		mMeshes.push_back(m);
	}

	vector<AglSkeletonMapping*> mappings = mAglScene->getSkeletonMappings();
	for (unsigned int i = 0; i < mappings.size(); i++)
	{
		SkeletonMapping* sm = new SkeletonMapping(mDevice, mappings[i]);
		mSkeletonMappings.push_back(sm);
	}

	vector<AglMaterial*> materials = mAglScene->getMaterials();
	for (unsigned int i = 0; i < materials.size(); i++)
	{
		string path = pPath;
		int ind = path.find_last_of('\\');
		if (ind == -1)
			ind = path.find_last_of('/');
		path = path.substr(0, ind+1);	

		if (materials[i]->diffuseTextureNameIndex >= 0)
		{
			string difpath = mAglScene->getName(materials[i]->diffuseTextureNameIndex);
			int ind = TextureManager::GetInstance()->LoadTexture(difpath);
			if (ind < 0)
			{
				materials[i]->diffuseTextureNameIndex = -1;
			}
			else
			{
				TextureData* data = TextureManager::GetInstance()->GetTexture(ind);
				mAglScene->setName(materials[i]->diffuseTextureNameIndex, data->Path);
			}
		}
		if (materials[i]->specularTextureNameIndex >= 0)
		{
			string specpath = mAglScene->getName(materials[i]->specularTextureNameIndex);
			int ind = TextureManager::GetInstance()->LoadTexture(specpath);
			if (ind < 0)
			{
				materials[i]->specularTextureNameIndex = -1;
			}
			else
			{
				TextureData* data = TextureManager::GetInstance()->GetTexture(ind);
				mAglScene->setName(materials[i]->specularTextureNameIndex, data->Path);
			}
		}
		if (materials[i]->normalTextureNameIndex >= 0)
		{
			string normpath = mAglScene->getName(materials[i]->normalTextureNameIndex);
			int ind = TextureManager::GetInstance()->LoadTexture(normpath);
			if (ind < 0)
			{
				materials[i]->normalTextureNameIndex = -1;
			}
			else
			{
				TextureData* data = TextureManager::GetInstance()->GetTexture(ind);
				mAglScene->setName(materials[i]->normalTextureNameIndex, data->Path);
			}
		}
		if (materials[i]->glowTextureNameIndex >= 0)
		{
			string glowpath = mAglScene->getName(materials[i]->glowTextureNameIndex);
			int ind = TextureManager::GetInstance()->LoadTexture(glowpath);
			if (ind < 0)
			{
				materials[i]->glowTextureNameIndex = -1;
			}
			else
			{
				TextureData* data = TextureManager::GetInstance()->GetTexture(ind);
				mAglScene->setName(materials[i]->glowTextureNameIndex, data->Path);
			}
		}
	}

	vector<AglMaterialMapping> mm = mAglScene->getMaterialMappings();
	for (unsigned int i = 0; i < mm.size(); i++)
	{
		mMeshes[mm[i].meshID]->AddMaterial(mm[i].materialID, true);
	}

	vector<SkeletonMesh*> skeletons;
	for (unsigned int i = 0; i < mAglScene->getSkeletonCount(); i++)
	{
		mSkeletonMeshes.push_back(new SkeletonMesh(mDevice, mDeviceContext, mAglScene->getSkeleton(i)));
	}

	vector<AglLooseBspTree*> bspTrees = mAglScene->getBspTrees();
	for (unsigned int i = 0; i < bspTrees.size(); i++)
	{
		mMeshes[bspTrees[i]->getHeader().targetMesh]->SetBspTree(bspTrees[i]);
	}
	vector<AglInteriorSphereGrid*> grids = mAglScene->getSphereGrids();
	for (unsigned int i = 0; i < grids.size(); i++)
	{
		mMeshes[grids[i]->getHeader().targetMesh]->SetInteriorSpheres(grids[i]);
	}

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
	{
		m_avoidJump = AglMatrix::identityMatrix();
		m_world = AglMatrix::identityMatrix();
	}


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
	vector<AglParticleSystem*> ps = mAglScene->getParticleSystems();
	for (unsigned int i = 0; i < ps.size(); i++)
	{
		mParticleSystems.push_back(new ParticleSystem(ps[i], mDevice, mDeviceContext));

		if (ps[i]->getHeader().textureNameIndex >= 0)
		{
			string path = mAglScene->getName(ps[i]->getHeader().textureNameIndex);
			int ind = TextureManager::GetInstance()->LoadTexture(path);
			if (ind < 0)
			{
				ps[i]->setTextureNameIndex(-1);
			}
			else
			{
				TextureData* data = TextureManager::GetInstance()->GetTexture(ind);
				mAglScene->setName(ps[i]->getHeader().textureNameIndex, data->Path);
			}
		}
	}

	if (mAglScene->getAnimationCount() > 0)
	{
		AglAnimation* anim = mAglScene->getAnimation(0);
		anim->play();
	}
}
/*void Scene::Init(vector<Mesh*> pMeshes, vector<SkeletonMesh*> pSkeletons, vector<SkeletonMapping*> pSkeletonMappings, AglScene* pAglScene, string pFolder,
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
	{
		m_avoidJump = AglMatrix::identityMatrix();
		m_world = AglMatrix::identityMatrix();
	}

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
	vector<AglParticleSystem*> ps = mAglScene->getParticleSystems();
	for (unsigned int i = 0; i < ps.size(); i++)
	{
		mParticleSystems.push_back(new ParticleSystem(ps[i], mDevice, mDeviceContext));

		if (ps[i]->getHeader().textureNameIndex >= 0)
		{
			string textureFile = mAglScene->getName(ps[i]->getHeader().textureNameIndex);
			removePath(textureFile);
			textureFile = pFolder + textureFile;
			TextureManager::GetInstance()->LoadTexture(textureFile);
		}
	}

	if (mAglScene->getAnimationCount() > 0)
	{
		AglAnimation* anim = mAglScene->getAnimation(0);
		anim->play();
	}
	CreateScenePlane();
}*/
void Scene::Update(float pElapsedTime)
{
	for (unsigned int i = 0; i < mMeshes.size(); i++)
		mMeshes[i]->update(pElapsedTime);
	if (mAglScene)
		mAglScene->update(pElapsedTime, Camera::GetInstance()->GetPosition());
	mRotation += pElapsedTime;
}
void Scene::Draw()
{
	if (!mAglScene)
		return;
	float maxV = max(max(mMax.x - mMin.x, mMax.y - mMin.y), mMax.z - mMin.z);
	float invMax = 1.0f / maxV;
	
	AglMatrix w;

	AglVector3 scale(1, 1, 1);
	AglMatrix::componentsToMatrix(w, scale, mQuaternionRotation, mPosition);

	if (mAglScene)
	{
		AglMatrix w3 = mAglScene->getCoordinateSystemAsMatrix().inverse();
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

		AglMatrix meshTransform = mMeshes[i]->getTransform();

		if (SPHEREMESH && DRAWDEBUGSPHERE)
		{
			AglBoundingSphere bs = mMeshes[i]->getBoundingSphere();
			AglMatrix sw;
			AglMatrix::componentsToMatrix(sw, AglVector3(bs.radius, bs.radius, bs.radius), AglQuaternion::identity(), bs.position);
			sw = sw * m_avoidJump;
			sw = sw * invMax;
			sw *= meshTransform;
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
			sw *= meshTransform;
			sw *= w;
			sw.SetTranslation(sw.GetTranslation() + w.GetTranslation());
			BOXMESH->Draw(sw, mBoxColors[i]);
		}
	}
	for (unsigned int i = 0; i < mSkeletonMeshes.size(); i++)
		mSkeletonMeshes[i]->Draw(w, invMax);

	//Get Particle systems
	vector<pair<ParticleSystem*, float>> sortedPS;
	for (unsigned int i = 0; i < mParticleSystems.size(); i++)
	{
		float dist = mParticleSystems[i]->getCameraDistance(Camera::GetInstance()->GetPosition(), Camera::GetInstance()->LocalZAxis());
		sortedPS.push_back(pair<ParticleSystem*, float>(mParticleSystems[i], dist));
	}

	//Sort Particle systems
	for (unsigned int i = 0; i < sortedPS.size(); i++)
	{
		for (unsigned int j = 1; j < sortedPS.size(); j++)
		{
			if (sortedPS[j].second > sortedPS[j-1].second)
			{
				pair<ParticleSystem*, float> temp = sortedPS[j];
				sortedPS[j] = sortedPS[j-1];
				sortedPS[j-1] = temp;
			}
		}
	}


	//Draw Particle systems
	for (unsigned int i = 0; i < sortedPS.size(); i++)
	{
		sortedPS[i].first->Draw();
	}


	vector<AglConnectionPoint> cp = mAglScene->getConnectionPoints();
	for (unsigned int i = 0; i < cp.size(); i++)
	{
		int parent = cp[i].parentMesh;
		AglMatrix t = AglMatrix::identityMatrix();
		if (parent >= 0)
		{
			t = mMeshes[parent]->getTransform();
		}
		t = cp[i].transform*t;
		t.SetTranslation(t.GetTranslation()*invMax);
		AglMatrix scale = AglMatrix::createScaleMatrix(AglVector3(0.1f, 0.001f, 0.001f));
		AglMatrix trans = AglMatrix::createTranslationMatrix(t.GetLeft() * 0.05f);
		BOXMESH->Draw(scale * t*trans, AglVector3(1, 0, 0));
		scale = AglMatrix::createScaleMatrix(AglVector3(0.001f, 0.1f, 0.001f));
		trans = AglMatrix::createTranslationMatrix(t.GetDown() * 0.05f);
		BOXMESH->Draw(scale * t*trans, AglVector3(0, 1, 0));
		scale = AglMatrix::createScaleMatrix(AglVector3(0.001f, 0.001f, 0.1f));
		trans = AglMatrix::createTranslationMatrix(t.GetBackward() * 0.05f);
		BOXMESH->Draw(scale * t*trans, AglVector3(0, 0, 1));


		//SPHEREMESH->Draw(t, AglVector3(1, 0, 1));
	}

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
int Scene::AddGradient(AglGradient* pGradient)
{
	return mAglScene->addGradient(pGradient);
}
void Scene::AddParticleSystem(AglParticleSystem* pSystem)
{
	mAglScene->addParticleSystem(pSystem);
	mParticleSystems.push_back(new ParticleSystem(pSystem, mDevice, mDeviceContext));
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
void Scene::Transform(AglMatrix p_transform)
{
	mAglScene->transform(p_transform);
	for (unsigned int i = 0; i < mMeshes.size(); i++)
		delete mMeshes[i];
	mMeshes.clear();
	vector<AglMesh*> meshes = mAglScene->getMeshes();
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		Mesh* m = new Mesh(mDevice, mDeviceContext, this);
		m->Init(meshes[i]);
		mMeshes.push_back(m);
	}

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
}
