#ifndef SKINPARSER_H
#define SKINPARSER_H

#include "FBXManager.h"
#include "AglSkeletonMapping.h"

class FBXParser;

class SkinParser
{
private:
	FBXParser* mParent;
	FbxGeometry* mGeometry;
public:
	SkinParser::SkinParser(FBXParser* pParent, FbxGeometry* pGeometry);
	void Parse();
	void ParseSkin(FbxSkin* pSkin);
	void ArrangeSkeleton(SkeletonData* pSkeleton);
};

#endif