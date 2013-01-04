#ifndef UTILITY_H
#define UTILITY_H

#define FBXSDK_SHARED
#define KFBX_PLUGIN
#define KFBX_FBXSDK
#define KFBX_NODLL
#define FBXSDK_NEW_API
#include <fbxsdk.h>
#include <fbxsdk\scene\shading\fbxsurfacematerial.h>
#include <math.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include "Agl.h"
#include <AglNodeAnimation.h>
#include <AglMesh.h>

using namespace std;

struct MeshData
{
	int				ID;
	string			Name;
	AglVertexFormat	VertexFormat;
	void*		Vertices;
	unsigned int*	Indices;
	int				VertexCount;
	int				IndicesCount;
	int*			ControlIndices;
	FbxMesh*		Source;
	FbxNode*		SourceNode;
};

struct NodeData
{
	int ID;
	FbxNode* SourceNode;
	FbxCluster* SourceCluster;

	float	InverseBindMatrix[16];
	float	LocalTransform[16];
};

struct JointData
{
	int ID;
	int NodeID;
	FbxNode* FbxParent;
	int Parent;
};

struct SkeletonData
{
	int					ID;
	string				Name;
	vector<JointData*>	Joints;
	FbxSkin*			Source;
};

struct NodeAnimationData
{
	int ID;
	int NodeID;
	vector<AglKeyFrame> KeyFrames;
};

struct AnimationLayerData
{
	int ID;
	FbxAnimLayer* Source;
	vector<unsigned int> NodeAnimations;
};

struct AnimationData
{
	int ID;
	FbxAnimStack*	Source;
	vector<unsigned int> Layers;
};

struct MaterialData
{
	int ID;
	FbxSurfaceMaterial* Source;
	float Ambient[3];
	float Diffuse[3];
	float Specular[3];
	float Emissive[3];
	float Opacity;
	float Shininess;
	float Reflectivity;

	string DiffuseTexture;
	string SpecularTexture;
};

#endif