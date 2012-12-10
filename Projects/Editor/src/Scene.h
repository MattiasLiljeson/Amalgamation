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
	vector<SkeletonMesh*> mSkeletonMeshes;
	vector<SkeletonMapping*> mSkeletonMappings;

	AglScene* mAglScene;

	AglVector3 mMin;
	AglVector3 mMax;

	float mRotation;
	AglQuaternion mQuaternionRotation;
	AglVector3 mPosition;

	AglMatrix m_world;
	int mCurrentAnimation;

	string mFolder;
private:
	Scene();
	~Scene();
public:
	static Scene* GetInstance();
	static void Release();
	void Init(vector<Mesh*> pMeshes, vector<SkeletonMesh*> pSkeletons, vector<SkeletonMapping*> pSkeletonMappings, AglScene* pAglScene, string pFolder);
	void Update(float pElapsedTime);
	void Draw();
	AglNode GetNode(int pIndex);

	vector<string> GetMeshNames();
	vector<string> GetMatrialNames();
	vector<AglMaterial*> GetMaterials();
	AglMaterial* GetMaterial(int pIndex);
	void SetCurrentAnimation(int pIndex);
	Mesh* GetMesh(int pIndex);
	vector<Mesh*> GetMeshes() { return mMeshes; }
	AglSkeleton* GetSkeleton(int pIndex);
	AglNodeAnimation* GetNodeAnimation(int pIndex);
	AglAnimationLayer* GetAnimationLayer(int pIndex);

	void AddMaterial(AglMaterial* pMaterial, bool pAddToMeshes, bool pSetAsCurrent);
	void AddGradient(AglGradient* pGradient, bool pAddToMeshes, bool pSetAsCurrent);
	vector<AglGradient*> GetGradients();
	string GetName(int pIndex);
	int   AddName(string pName);

	string GetFolder(){ return mFolder; }
	void Save(string pPath);

	AglQuaternion* GetQuaternionRotation(){ return &mQuaternionRotation; }
	AglVector3* GetPosition(){ return &mPosition; }
	AglMatrix GetWorld() { return m_world; }
};

#endif