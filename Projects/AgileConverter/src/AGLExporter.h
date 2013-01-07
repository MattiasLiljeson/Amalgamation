#ifndef AGLEXPORTER_H
#define AGLEXPORTER_H

#include "Utility.h"
#include "AglWriter.h"

class AGLExporter
{
private:
	char*		mPath;
	AglScene*	mScene;
	vector<AglMesh*> mMeshes;
	vector<AglNode> mNodes;
	vector<AglMaterial*> mMaterials;
	vector<AglMaterialMapping> mMaterialMappings;
	vector<AglSkeleton*> mSkeletons;
	vector<AglSkeletonMapping*> mMeshSkeletonMappings;
	vector<AglNodeAnimation*> mNodeAnimations;
	vector<AglAnimationLayer*> mAnimationLayers;
	vector<AglAnimation*> mAnimations;
public:
	AGLExporter(string pPath);
	~AGLExporter();
	void AddMesh(MeshData* pData);
	void AddNode(NodeData* pData);
	void AddSkeleton(SkeletonData* pData);
	void AddMeshSkeletonMapping(AglSkeletonMapping* pData);
	void AddNodeAnimation(NodeAnimationData* pData);
	void AddAnimationLayer(AnimationLayerData* pData);
	void AddAnimation(AnimationData* pData);
	void AddMaterial(MaterialData* pData);
	void AddMaterialMapping(AglMaterialMapping* pData);
	void AddConnectionPoint(AglConnectionPoint pCP);
	void Write();
};

#endif