#ifndef MESHPARSER_H
#define MESHPARSER_H

#include "FBXManager.h"

struct HashData
{
	int n;
	int t;
	int index;
};

class FBXParser;

class MeshParser
{
private:
	FBXParser* mParent;
	FbxMesh* mMesh;
	FbxNode* mNode;
	string mPath;

private:
	vector<AglVector3> GetPositions();
	vector<AglVector3> GetNormals();
	vector<AglVector3> GetTangents();
	vector<AglVector3> GetBitangents();
	vector<AglVector2> GetUV();
	void ComputeTangentSpace(vector<AglVertexSTBN>& pVertices, unsigned int* pIndices, int pIndexCount);

public:
	MeshParser(FBXParser* pParent, FbxMesh* pMesh, FbxNode* pNode);
	void Parse();
};

#endif