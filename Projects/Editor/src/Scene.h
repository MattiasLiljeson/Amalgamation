#ifndef SCENE_H
#define SCENE_H

#include "Utility.h"
#include "Agl.h"
#include <AglScene.h>
#include <AglGradient.h>

class Mesh;
class Skeleton;
class SkeletonMapping;
class Animation;
struct Material;
class Gradient;
class AnimationLayer;
class NodeAnimation;
struct Node;
struct MeshConfigurableData;
class SkeletonMesh;
class ParticleSystem;


///Need to add support for various 
/// coordinate systems

//Conversion can be handled by inverting
//the z axis for world transformation matrices and
//changing the cull mode to cull front

class Scene
{
private:
	static Scene* sInstance;

	vector<Mesh*> mMeshes;
	vector<ParticleSystem*> mParticleSystems;
	vector<AglVector3> mSphereColors;
	vector<AglVector3> mBoxColors;
	vector<SkeletonMesh*> mSkeletonMeshes;
	vector<SkeletonMapping*> mSkeletonMappings;

	AglScene* mAglScene;

	AglVector3 mMin;
	AglVector3 mMax;

	float mRotation;
	AglQuaternion mQuaternionRotation;
	AglVector3 mPosition;

public:
	static AglMatrix m_world;
	static AglMatrix m_avoidJump;
private:
	int mCurrentAnimation;

	string mFolder;

	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;

	Mesh* mPlaneMesh;

	vector<string> mPaths;

private:
	Scene();
	~Scene();

	void CreateScenePlane(); 
public:
	static Scene* GetInstance();
	static void Release();
	void Init(vector<Mesh*> pMeshes, vector<SkeletonMesh*> pSkeletons, vector<SkeletonMapping*> pSkeletonMappings, AglScene* pAglScene, string pFolder,
		ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void Update(float pElapsedTime);
	void Draw();
	AglNode GetNode(int pIndex);

	vector<string> GetMeshNames();
	vector<string> GetMatrialNames();
	vector<AglMaterial*> GetMaterials();
	AglMaterial* GetMaterial(int pIndex);
	void SetCurrentAnimation(int pIndex);
	Mesh* GetMesh(int pIndex);
	ParticleSystem* GetParticleSystem(int pIndex)
	{
		return mParticleSystems[pIndex];
	}
	vector<ParticleSystem*> GetParticleSystems()
	{
		return mParticleSystems;
	}
	vector<AglConnectionPoint> getConnectionPoints()
	{
		return mAglScene->getConnectionPoints();
	}
	vector<Mesh*> GetMeshes() { return mMeshes; }
	AglSkeleton* GetSkeleton(int pIndex);
	AglNodeAnimation* GetNodeAnimation(int pIndex);
	AglAnimationLayer* GetAnimationLayer(int pIndex);

	void AddMaterial(AglMaterial* pMaterial, bool pAddToMeshes, bool pSetAsCurrent);
	void AddGradient(AglGradient* pGradient, bool pAddToMeshes, bool pSetAsCurrent);
	void AddParticleSystem(AglParticleSystem* pSystem);
	vector<AglGradient*> GetGradients();
	string GetName(int pIndex);
	int   AddName(string pName);
	void  AddPath(string pPath, int pIndex)
	{
		while (pIndex >= mPaths.size())
			mPaths.push_back("");
		mPaths[pIndex] = pPath;
	}
	string GetPath(int pIndex)
	{
		return mPaths[pIndex];
	}

	string GetFolder(){ return mFolder; }
	void Save(string pPath);

	AglQuaternion* GetQuaternionRotation(){ return &mQuaternionRotation; }
	AglVector3* GetPosition(){ return &mPosition; }
	AglMatrix GetWorld() { return m_world; }
	AglVector3 GetCenter();

	bool IsLeftHanded();
	void SetCoordinateSystem(AglCoordinateSystem pSystem);

	void Transform(AglMatrix p_transform);

	AglScene* getAglScene(){ return mAglScene; }
};

#endif