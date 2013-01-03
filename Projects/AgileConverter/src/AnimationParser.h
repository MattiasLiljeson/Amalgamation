#ifndef ANIMATIONPARSER_H
#define ANIMATIONPARSER_H

#include "FBXManager.h"
#include "Utility.h"
#include "Agl.h"

class FBXParser;

class AnimationParser
{
private:
	FBXParser* mParent;

	vector<AnimationLayerData*> mAnimationLayers;
	vector<AnimationData*> mAnimations;
	vector<NodeAnimationData*> mNodeAnimations;

	int counter;
private:
	void GetStacks();
	void GetLayers();
	bool HasKey(vector<FbxTime>& pKeys, FbxTime pKey);
	void InsertKey(vector<FbxTime>& pKeys, FbxTime pKey);
public:
	AnimationParser(FBXParser* pParent);
	void Parse(vector<NodeData*> pNodes);
	void Parse(NodeData* pNode);
};

#endif