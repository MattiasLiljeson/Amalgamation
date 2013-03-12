#ifndef FBXPARSER_H
#define FBXPARSER_H

#include "MeshParser.h"
#include "SkinParser.h"
#include "AnimationParser.h"
#include "MaterialParser.h"

struct AGLData
{
	string OriginalPath;
	vector<MeshData*> Meshes; 
	vector<NodeData*> Nodes;
	vector<SkeletonData*> Skeletons;
	vector<AglSkeletonMapping*> MeshSkeletonMappings;
	vector<NodeAnimationData*> NodeAnimations;
	vector<AnimationLayerData*> AnimationLayers;
	vector<AnimationData*> Animations;
	vector<MaterialData*> Materials;
	vector<AglMaterialMapping*> MaterialMappings;
	vector<pair<AglConnectionPoint, string>> CP;
};

class FBXParser
{
private:
	FbxScene* mScene;
	
	AGLData mData;
	/*vector<NodeData*> mNodes;
	vector<SkeletonData*> mSkeletons;
	vector<AglSkeletonMapping*> mMeshSkeletonMappings;
	vector<NodeAnimationData*> mNodeAnimations;
	vector<AnimationLayerData*> mAnimationLayers;
	vector<AnimationData*> mAnimations;
	vector<MaterialData*> mMaterials;
	vector<AglMaterialMapping*> mMaterialMappings;
	vector<pair<AglConnectionPoint, string>> mConnectionPoints;*/


private:
	void ParseNode(FbxNode* pNode);
public:
	FBXParser(string pPath);
	~FBXParser();
	void Parse();

	void AddMesh(MeshData* pMesh);
	void AddSkeleton(SkeletonData* pSkeleton);
	void AddNodeAnimation(NodeAnimationData* pNodeAnimationData);
	void AddAnimationLayer(AnimationLayerData* pAnimationLayer);
	void AddAnimation(AnimationData* pAnimation);
	void AddMaterial(MaterialData* pMaterial);

	void CreateMapping(MeshData* pMesh, SkeletonData* pSkeleton);

	vector<MeshData*> GetMeshes();
	vector<NodeData*> GetNodes();
	vector<SkeletonData*> GetSkeletons();
	vector<AglSkeletonMapping*> GetMeshSkeletonMappings();
	vector<NodeAnimationData*> GetNodeAnimations();
	vector<AnimationLayerData*> GetAnimationLayers();
	vector<AnimationData*> GetAnimations();
	vector<MaterialData*> GetMaterials();
	vector<AglMaterialMapping*> GetMaterialMappings();
	vector<pair<AglConnectionPoint, string>> GetConnectionPoints();

	FbxScene* GetScene();

	unsigned int GetNodeID(FbxNode* pNode, FbxCluster* pCluster = NULL);
	NodeData* GetNode(int pIndex);
	MaterialData* GetMaterial(FbxSurfaceMaterial* pMaterial);

	unsigned int FindIndex(FbxAnimLayer* pAnimLayer);

	AGLData GetData(){ return mData; }

	void offsetNodeTransform(int p_node, int p_parent);
};

#endif