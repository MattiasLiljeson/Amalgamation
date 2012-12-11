#ifndef MATERIALPARSER_H
#define MATERIALPARSER_H

#include "FBXManager.h"
#include "Utility.h"
#include "Agl.h"

class FBXParser;

class MaterialParser
{
private:
	FBXParser* mParent;

private:
	string ParseTexture(FbxFileTexture* pTexture);
public:
	MaterialParser(FBXParser* pParent);
	void Parse(FbxSurfaceMaterial* pMaterial);
};

#endif