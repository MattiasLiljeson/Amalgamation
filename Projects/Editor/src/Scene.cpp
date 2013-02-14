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

void Scene::MaterialFromStrings(vector<string> pStrings)
{
	vector<string> lookFor;
	lookFor.push_back("diffuse");
	lookFor.push_back("specular");
	lookFor.push_back("normal");
	lookFor.push_back("emissive");
	lookFor.push_back("gradient");

	AglMaterial* mat = mAglScene->getMaterial(0);

	for (unsigned int i = 0; i < pStrings.size(); i++)
	{
		if (pStrings[i].find(lookFor[0]) != string::npos) //Diffuse
		{
			int ind = TextureManager::GetInstance()->LoadTexture(pStrings[i]);
			TextureData* data = TextureManager::GetInstance()->GetTexture(ind);
			ind = AddName(pStrings[i]);
			mat->diffuseTextureNameIndex = ind;
		}
		else if (pStrings[i].find(lookFor[1]) != string::npos) //Specular
		{
			int ind = TextureManager::GetInstance()->LoadTexture(pStrings[i]);
			TextureData* data = TextureManager::GetInstance()->GetTexture(ind);
			ind = AddName(pStrings[i]);
			mat->specularTextureNameIndex = ind;
		}
		else if (pStrings[i].find(lookFor[2]) != string::npos) //Normal
		{
			int ind = TextureManager::GetInstance()->LoadTexture(pStrings[i]);
			TextureData* data = TextureManager::GetInstance()->GetTexture(ind);
			ind = AddName(pStrings[i]);
			mat->normalTextureNameIndex = ind;
		}
		else if (pStrings[i].find(lookFor[3]) != string::npos) //Emissive
		{
			int ind = TextureManager::GetInstance()->LoadTexture(pStrings[i]);
			TextureData* data = TextureManager::GetInstance()->GetTexture(ind);
			ind = AddName(pStrings[i]);
			mat->glowTextureNameIndex = ind;
		}
		else if (pStrings[i].find(lookFor[4]) != string::npos) //Gradient
		{
			int ind = TextureManager::GetInstance()->LoadTexture(pStrings[i]);
			TextureData* data = TextureManager::GetInstance()->GetTexture(ind);
			ind = AddName(pStrings[i]);
			mat->gradientTextureNameIndex = ind;
		}
	}

	//AddMaterial(mat, true, true);
}

vector<string> Scene::FindFiles(string pPath)
{	
	string thefolder;
	int ind = pPath.find_last_of('\\');
	if (ind == -1)
		ind = pPath.find_last_of('/');
	if (ind == -1)
		thefolder = ".";
	else
	{
		thefolder = pPath.substr(0, ind);
	}

	thefolder += "\\*.*";

	WIN32_FIND_DATA file;
	HANDLE folder = FindFirstFile(thefolder.c_str(),&file);

	vector<string> files;
	if(folder != INVALID_HANDLE_VALUE) 
	{
		do
		{
			char* nPtr = new char[lstrlen(file.cFileName) + 1];

			for (int i = 0; i < lstrlen(file.cFileName); i++)
			{
				nPtr[i] = char(file.cFileName[i]);
			}

			nPtr[lstrlen(file.cFileName)] = '\0';

			files.push_back(string(nPtr));
			delete[] nPtr;

		} while(FindNextFile(folder, &file));
	} 
	else 
	{
		cout << "Error: No such folder." << endl;
	}

	FindClose(folder);

	for (int i = files.size()-1; i >= 0; i--)
	{
		if (files[i].size() < 3)
		{
			files[i] = files.back();
			files.pop_back();
		}
		else
		{
			string end = files[i].substr(files[i].size()-3, 3);
			if (end != "png" && end != "tga")
			{
				files[i] = files.back();
				files.pop_back();
			}
		}
	}

	vector<string> lookFor;
	lookFor.push_back("diffuse");
	lookFor.push_back("specular");
	lookFor.push_back("normal");
	lookFor.push_back("emissive");
	lookFor.push_back("gradient");

	vector<string> found;

	AglMaterial* defaultMat = mAglScene->getMaterial(0);

	for (unsigned int i = 0; i < files.size(); i++)
	{
		std::transform(files[i].begin(), files[i].end(), files[i].begin(), ::tolower);

		for (unsigned j = 0; j < lookFor.size(); j++)
		{
			if (files[i].find(lookFor[j]) != string::npos)
			{
				if (j == 0 && defaultMat->diffuseTextureNameIndex >= 0)
					break;
				else if (j == 1 && defaultMat->specularTextureNameIndex >= 0)
					break;
				else if (j == 2 && defaultMat->normalTextureNameIndex >= 0)
					break;
				else if (j == 3 && defaultMat->glowTextureNameIndex >= 0)
					break;
				else if (j == 4 && defaultMat->gradientTextureNameIndex >= 0)
					break;
				found.push_back(files[i]);
				break;
			}
		}
	}

	if (found.size() > 0)
	{
		string founds;
		for (unsigned int i = 0; i < found.size(); i++)
		{
			founds += "'" + found[i] + "'\n";
		}

		string msg = "Found the following:\n" + founds + "textures in the root folder. Use on default material?";
		int proceed = MessageBox(NULL, msg.c_str(), "Found Textures", MB_OKCANCEL);
		if (proceed == IDOK)
		{
			MaterialFromStrings(found);
		}
	}

	return files;
}

Scene::Scene()
{
	mRotation = 0;
	mCurrentAnimation = 0;
	mAglScene = NULL;
	mQuaternionRotation = AglQuaternion(0, 0, 0, 1);
	mPosition = AglVector3(0, 0, 0);
	mDrawPlanes = true;
	conpointLength = 0.1f;
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
	mPath = pPath;

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
		if (materials[i]->gradientTextureNameIndex >= 0)
		{
			string gradpath = mAglScene->getName(materials[i]->gradientTextureNameIndex);
			int ind = TextureManager::GetInstance()->LoadTexture(gradpath);
			if (ind < 0)
			{
				materials[i]->gradientTextureNameIndex = -1;
			}
			else
			{
				TextureData* data = TextureManager::GetInstance()->GetTexture(ind);
				mAglScene->setName(materials[i]->gradientTextureNameIndex, data->Path);
			}
		}
	}

	vector<AglMaterialMapping> mm = mAglScene->getMaterialMappings();
	for (unsigned int i = 0; i < mm.size(); i++)
	{
		mMeshes[mm[i].meshID]->SetMaterial(mm[i].materialID);
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

	createScenePlane();
	FindFiles(pPath);
}
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
	if (mMeshes.size() == 0)
		invMax = 1.0f;
	
	AglMatrix w;

	AglVector3 scale(1, 1, 1);
	AglMatrix::componentsToMatrix(w, scale, mQuaternionRotation, mPosition);

	if (mAglScene)
	{
		AglMatrix w3 = mAglScene->getCoordinateSystemAsMatrix().inverse();
		w = w3 * w;
	}

	//AglMatrix::MatrixToComponents(w2, v1, mQuaternionRotation, v2);
	for (unsigned int i = 0; i < mMeshes.size(); i++)
	{
		mMeshes[i]->Draw(w, invMax);

		AglMatrix meshTransform = mMeshes[i]->getTransform();

		if (SPHEREMESH && DRAWDEBUGSPHERE)
		{
			AglBoundingSphere bs = mMeshes[i]->getBoundingSphere();
			AglMatrix sw;
			AglMatrix::componentsToMatrix(sw, AglVector3(bs.radius, bs.radius, bs.radius), AglQuaternion::identity(), bs.position);
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

	//Connection points
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
		AglMatrix scale = AglMatrix::createScaleMatrix(AglVector3(conpointLength, 0.001f, 0.001f));
		AglMatrix trans = AglMatrix::createTranslationMatrix(t.GetLeft() * conpointLength*0.5f);
		BOXMESH->Draw(scale * t*trans, AglVector3(1, 0, 0));
		scale = AglMatrix::createScaleMatrix(AglVector3(0.001f, conpointLength, 0.001f));
		trans = AglMatrix::createTranslationMatrix(t.GetDown() * conpointLength*0.5f);
		BOXMESH->Draw(scale * t*trans, AglVector3(0, 1, 0));
		scale = AglMatrix::createScaleMatrix(AglVector3(0.001f, 0.001f, conpointLength));
		trans = AglMatrix::createTranslationMatrix(t.GetBackward() * conpointLength*0.5f);
		BOXMESH->Draw(scale * t*trans, AglVector3(0, 0, 1));
	}

	if (mDrawPlanes)
	{
		planeDown->Draw(AglMatrix::identityMatrix());
		//planeRight->Draw(AglMatrix::identityMatrix());
		//planeBack->Draw(AglMatrix::identityMatrix());

		//X-Axis
		AglMatrix scale = AglMatrix::createScaleMatrix(AglVector3(1.0f, 0.001f, 0.001f));
		AglMatrix trans = AglMatrix::createTranslationMatrix(GetCenter()-AglVector3(0.0f, 0.5f, 0.5f));
		BOXMESH->Draw(scale * trans, AglVector3(1, 0, 0));

		//Y-Axis
		scale = AglMatrix::createScaleMatrix(AglVector3(0.001f, 1.0f, 0.001f));
		trans = AglMatrix::createTranslationMatrix(GetCenter()-AglVector3(0.5f, 0.0f, 0.5f));
		BOXMESH->Draw(scale * trans, AglVector3(0, 1, 0));

		//Z-Axis
		scale = AglMatrix::createScaleMatrix(AglVector3(0.001f, 0.001f, 1.0f));
		trans = AglMatrix::createTranslationMatrix(GetCenter()-AglVector3(0.5f, 0.5f, 0.0f));
		BOXMESH->Draw(scale * trans, AglVector3(0, 0, 1));

	}

	if (BOXMESH && mBoxColors.size() > 0)
	{
		AglOBB sceneOBB = mAglScene->getSceneOBB();

		AglMatrix sw = sceneOBB.world;
		AglMatrix size;
		AglMatrix::componentsToMatrix(size, sceneOBB.size, AglQuaternion::identity(), AglVector3(0, 0, 0));
		sw = size * sw;
		sw = sw * invMax;
		sw *= w;
		sw.SetTranslation(sw.GetTranslation() + w.GetTranslation());
		BOXMESH->Draw(sw, mBoxColors[0]);
	}

	//Draw Particle systems
	for (unsigned int i = 0; i < sortedPS.size(); i++)
	{
		sortedPS[i].first->Draw();
	}
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
	if (pAddToMeshes)
	{
		for (unsigned int i = 0; i < mMeshes.size(); i++)
		{
			AglMaterialMapping mm;
			mm.materialID = pMaterial->id;
			mm.meshID = i;
			mAglScene->addMaterialMapping(mm);
			if (pSetAsCurrent)
				mMeshes[i]->SetMaterial(pMaterial->id);
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
	if (pPath == "")
		pPath = mPath;
	AglWriter w(pPath);
	w.write(mAglScene);
}
AglVector3 Scene::GetCenter() 
{
	AglOBB obb = mAglScene->getSceneOBB();
	float divideWith = max(obb.size.x, max(obb.size.y, obb.size.z));
	return obb.world.GetTranslation() / divideWith;
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
	vector<int> mats;
	for (unsigned int i = 0; i < mMeshes.size(); i++)
	{
		mats.push_back(mMeshes[i]->getCurrentMaterial());
		delete mMeshes[i];
	}
	mMeshes.clear();
	vector<AglMesh*> meshes = mAglScene->getMeshes();
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		Mesh* m = new Mesh(mDevice, mDeviceContext, this);
		m->Init(meshes[i]);
		m->SetMaterial(mats[i]);
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

int  Scene::GetIndex(AglParticleSystem* pSystem)
{
	for (unsigned int i = 0; i < mParticleSystems.size(); i++)
	{
		if (mParticleSystems[i]->getParticleSystem() == pSystem)
			return i;
	}
	return -1;
}

void Scene::RemoveMaterial(AglMaterial* pMaterial)
{
	mAglScene->RemoveMaterial(pMaterial);

	//Remove degenerate materials from meshes
	for (unsigned int i = 0; i < mMeshes.size(); i++)
	{
		mMeshes[i]->SetMaterial(-1);
	}

	//Add updated materials
	vector<AglMaterialMapping> mm = mAglScene->getMaterialMappings();
	for (unsigned int i = 0; i < mm.size(); i++)
	{
		mMeshes[mm[i].meshID]->SetMaterial(mm[i].materialID);
	}
}
void Scene::RemoveParticleSystem(AglParticleSystem* pParticleSystem)
{
	for (unsigned int i = 0; i < mParticleSystems.size(); i++)
	{
		if (mParticleSystems[i]->getParticleSystem() == pParticleSystem)
		{
			delete mParticleSystems[i];
			mParticleSystems[i] = mParticleSystems.back();
			mParticleSystems.pop_back();
			break;
		}
	}
	mAglScene->RemoveParticleEffect(pParticleSystem);
}
void Scene::createScenePlane()
{
	//Plane Down
	planeDown = new Mesh(mDevice, mDeviceContext, this);
	AglVector3 c = GetCenter();
	AglMeshHeader h;
	h.indexCount = 6;
	h.vertexCount = 4;
	h.transform = AglMatrix::identityMatrix();
	h.vertexFormat = AGL_VERTEX_FORMAT_PNTTBN;

	AglVertexSTBN* vertices = new AglVertexSTBN[4];
	vertices[0].position = c+AglVector3(0.5f, -0.5f, 0.5f);	vertices[0].normal = AglVector3(0, 1, 0);
	vertices[1].position = c+AglVector3(0.5f, -0.5f, -0.5f);	vertices[1].normal = AglVector3(0, 1, 0);
	vertices[2].position = c+AglVector3(-0.5f, -0.5f, -0.5f);	vertices[2].normal = AglVector3(0, 1, 0);
	vertices[3].position = c+AglVector3(-0.5f, -0.5f, 0.5f); 	vertices[3].normal = AglVector3(0, 1, 0);

	unsigned int* indices = new unsigned int[6];

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	AglMesh* mesh = new AglMesh(h, vertices, indices);

	planeDown->Init(mesh);

	AglMaterial* mat = new AglMaterial();
	mat->diffuse = AglVector3(0, 0.5f, 0.5f);
	mat->specular = AglVector3(0.0f, 0.0f, 0.0f);
	planeDown->setOverrideMaterial(mat);

	//Plane Right
	planeRight = new Mesh(mDevice, mDeviceContext, this);

	vertices = new AglVertexSTBN[4];
	vertices[0].position = c+AglVector3(0.5f, 0.5f, 0.5f);	vertices[0].normal = AglVector3(-1, 0, 0);
	vertices[1].position = c+AglVector3(0.5f, 0.5f, -0.5f);	vertices[1].normal = AglVector3(-1, 0, 0);
	vertices[2].position = c+AglVector3(0.5f, -0.5f, -0.5f);	vertices[2].normal = AglVector3(-1, 0, 0);
	vertices[3].position = c+AglVector3(0.5f, -0.5f, 0.5f); 	vertices[3].normal = AglVector3(-1, 0, 0);

	indices = new unsigned int[6];

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	mesh = new AglMesh(h, vertices, indices);

	planeRight->Init(mesh);

	//Plane Back
	planeBack = new Mesh(mDevice, mDeviceContext, this);

	vertices = new AglVertexSTBN[4];
	vertices[0].position = c+AglVector3(0.5f, 0.5f, 0.5f);	vertices[0].normal = AglVector3(0, 0, -1);
	vertices[1].position = c+AglVector3(-0.5f, 0.5f, 0.5f);	vertices[1].normal = AglVector3(0, 0, -1);
	vertices[2].position = c+AglVector3(-0.5f, -0.5f, 0.5f);	vertices[2].normal = AglVector3(0, 0, -1);
	vertices[3].position = c+AglVector3(0.5f, -0.5f, 0.5f); 	vertices[3].normal = AglVector3(0, 0, -1);

	indices = new unsigned int[6];

	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;
	indices[3] = 2;
	indices[4] = 0;
	indices[5] = 3;

	mesh = new AglMesh(h, vertices, indices);

	planeBack->Init(mesh);
}
