#include "FBXParser.h"

FBXParser::FBXParser(string pPath)
{
	mPath = pPath;
}
FBXParser::~FBXParser()
{
	for (unsigned int i = 0; i < mMeshes.size(); i++)
	{
		delete[] mMeshes[i]->Vertices;
		delete[] mMeshes[i]->Indices;
		delete mMeshes[i];
	}
}

void FBXParser::Parse()
{
	FbxManager* manager = FBXManager::GetInstance()->GetManager();
	FbxImporter* importer = FbxImporter::Create(manager,"");
    
	if(!importer->Initialize(mPath.c_str(), -1, manager->GetIOSettings())) 
	{
		cout << "Failed to initialize Importer";
        //cout << importer->GetLastErrorString() << "\n\n";
        return;
    }
	
	FbxAxisSystem dx(FbxAxisSystem::eDirectX);
	FbxAxisSystem gl(FbxAxisSystem::eOpenGL);
	FbxAxisSystem max(FbxAxisSystem::eMax);
	FbxAxisSystem mayay(FbxAxisSystem::eMayaYUp);
	FbxAxisSystem mayaz(FbxAxisSystem::eMayaZUp);
	FbxAxisSystem light(FbxAxisSystem::eLightwave);
	FbxAxisSystem motion(FbxAxisSystem::eMotionBuilder);

	//dx = light
	//gl = mayay = motion
	//max = mayaz

    mScene = FbxScene::Create(manager, "");

	importer->Import(mScene);

	FbxAxisSystem SceneAxisSystem = mScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem OurAxisSystem(FbxAxisSystem::eDirectX);

	if (SceneAxisSystem != OurAxisSystem)
	{
		//OurAxisSystem.ConvertScene(scene);
	}

	for (int i = 0; i < mScene->GetNodeCount(); i++)
	{
		const char* n = mScene->GetNode(i)->GetName();
		ParseNode(mScene->GetNode(i));
	}

	//Find NUll nodes
	vector<const char*> nodeNames;
	for (int i = 0; i < mScene->GetNodeCount(); i++)
	{
		FbxNode* node = mScene->GetNode(i);
		const char* n = node->GetName();
		FbxNodeAttribute* attr = node->GetNodeAttribute();
		if (attr)
		{
			FbxNodeAttribute::EType attrType = attr->GetAttributeType();
			if (attrType == FbxNodeAttribute::eNull)
			{
				AglConnectionPoint cp;

				cp.parentMesh = -1;
				for (unsigned int i = 0; i < mMeshes.size(); i++)
				{
					if (node->GetParent() == mMeshes[i]->SourceNode)
					{
						cp.parentMesh = i;
						break;
					}
				}

				nodeNames.push_back(node->GetParent()->GetName()); 
				FbxMatrix transform = node->EvaluateLocalTransform();
				for (int row = 0; row < 4; row++)
					for (int column = 0; column < 4; column++)
						cp.transform[row*4 + column] = (float)transform.Get(row, column);
				mConnectionPoints.push_back(cp);
			}
		}
	}

	AnimationParser ap(this);
	ap.Parse(mNodes);

	//Create mappings between geometry and other data
	for (unsigned int i = 0; i < mMeshes.size(); i++)
	{
		//Create mapping between mesh and skeleton
		FbxGeometry* geometry = mMeshes[i]->Source;
		int skins = geometry->GetDeformerCount(FbxDeformer::eSkin);
		for (int j = 0; j < skins; j++)
		{
			FbxSkin* s = (FbxSkin*)geometry->GetDeformer(j, FbxDeformer::eSkin);
			for (unsigned int k = 0; k < mSkeletons.size(); k++)
			{
				if (s == mSkeletons[k]->Source)
				{
					CreateMapping(mMeshes[i], mSkeletons[k]);
				}
			}
		}

		//Create Mapping between mesh and material
		FbxNode* node = mMeshes[i]->SourceNode;
		int materials = node->GetMaterialCount();
		for (int j = 0; j < materials; j++)
		{
			FbxSurfaceMaterial* mat = node->GetMaterial(j);
			for (int k = 0; k < mMaterials.size(); k++)
			{
				if (mat == mMaterials[k]->Source)
				{
					AglMaterialMapping* mapping = new AglMaterialMapping();
					mapping->materialID = k;
					mapping->meshID = i;
					mMaterialMappings.push_back(mapping);
				}
			}
		}
	}

	mScene->Destroy();
    importer->Destroy();
}
void FBXParser::ParseNode(FbxNode* pNode)
{
	FbxNodeAttribute* attr = pNode->GetNodeAttribute();
	if (attr)
	{
		FbxNodeAttribute::EType attrType = pNode->GetNodeAttribute()->GetAttributeType();
		if (attrType == FbxNodeAttribute::eMesh)
		{
			//Parse the mesh itself
			FbxMesh* mesh = (FbxMesh*)attr;
			MeshParser mp(this, mesh, pNode);
			mp.Parse();

			//Parse the skeleton of the mesh
			FbxGeometry* geometry = (FbxGeometry*)mesh;
			SkinParser sp(this, geometry);
			sp.Parse();

			int mats = pNode->GetMaterialCount();
			for (int i = 0; i < mats; i++)
			{
				MaterialParser mp(this);
				mp.Parse(pNode->GetMaterial(i));
			}
		}
	}
}

void FBXParser::AddMesh(MeshData* pMesh)
{
	pMesh->ID = mMeshes.size();
	mMeshes.push_back(pMesh);
}
void FBXParser::AddSkeleton(SkeletonData* pSkeleton)
{
	pSkeleton->ID = mSkeletons.size();
	mSkeletons.push_back(pSkeleton);
}
void FBXParser::AddNodeAnimation(NodeAnimationData* pNodeAnimationData)
{
	pNodeAnimationData->ID = mNodeAnimations.size();
	mNodeAnimations.push_back(pNodeAnimationData);
}
void FBXParser::AddAnimationLayer(AnimationLayerData* pAnimationLayer)
{
	pAnimationLayer->ID = mAnimationLayers.size();
	mAnimationLayers.push_back(pAnimationLayer);
}
void FBXParser::AddAnimation(AnimationData* pAnimation)
{
	pAnimation->ID = mAnimations.size();
	mAnimations.push_back(pAnimation);
}
void FBXParser::AddMaterial(MaterialData* pMaterial)
{
	pMaterial->ID = mMaterials.size();
	mMaterials.push_back(pMaterial);
}

void FBXParser::CreateMapping(MeshData* pMesh, SkeletonData* pSkeleton)
{
	AglSkeletonMappingHeader smh;
	smh.mappingType = 0;
	smh.meshID = pMesh->ID;
	smh.skeletonID = pSkeleton->ID;
	smh.vertexCount = pMesh->VertexCount;

	vector<int> sizes(smh.vertexCount, 0);
	AglSkeletonMappingVertex* verts = new AglSkeletonMappingVertex[pMesh->Source->GetControlPointsCount()];
	for (int i = 0; i < pMesh->Source->GetControlPointsCount(); i++)
	{
		for (int j = 0; j < 8; j++)
		{
			verts[i].indices[j] = 0;
			verts[i].weights[j] = 0;
		}
	}

	int clusters = pSkeleton->Joints.size();
	for (int i = 0; i < clusters; i++)
	{
		if (mNodes[pSkeleton->Joints[i]->NodeID]->SourceCluster)
		{
			FbxCluster* c	= mNodes[pSkeleton->Joints[i]->NodeID]->SourceCluster;
			int indexCount	= c->GetControlPointIndicesCount();
			int* indices	= c->GetControlPointIndices();
			double* weights = c->GetControlPointWeights();

			for(int j = 0; j < indexCount; j++)
			{
				int index = indices[j];
				verts[index].indices[sizes[index]] = i;
				verts[index].weights[sizes[index]] = (float)weights[j];
				sizes[index]++;
			}
		}
	}

	AglSkeletonMappingVertex* matchedVerts = new AglSkeletonMappingVertex[smh.vertexCount];
	for (int i = 0; i < smh.vertexCount; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			matchedVerts[i].indices[j] = verts[pMesh->ControlIndices[i]].indices[j];
			matchedVerts[i].weights[j] = verts[pMesh->ControlIndices[i]].weights[j];
		}
	}

	AglSkeletonMapping* m = new AglSkeletonMapping(smh, matchedVerts);
	mMeshSkeletonMappings.push_back(m);
}

vector<MeshData*> FBXParser::GetMeshes()
{
	return mMeshes;
}
vector<NodeData*> FBXParser::GetNodes()
{
	return mNodes;
}
vector<SkeletonData*> FBXParser::GetSkeletons()
{
	return mSkeletons;
}
vector<AglSkeletonMapping*> FBXParser::GetMeshSkeletonMappings()
{
	return mMeshSkeletonMappings;
}
vector<NodeAnimationData*> FBXParser::GetNodeAnimations()
{
	return mNodeAnimations;
}
vector<AnimationLayerData*> FBXParser::GetAnimationLayers()
{
	return mAnimationLayers;
}
vector<AnimationData*> FBXParser::GetAnimations()
{
	return mAnimations;
}
vector<MaterialData*> FBXParser::GetMaterials()
{
	return mMaterials;
}
vector<AglMaterialMapping*> FBXParser::GetMaterialMappings()
{
	return mMaterialMappings;
}

FbxScene* FBXParser::GetScene()
{
	return mScene;
}

unsigned int FBXParser::GetNodeID(FbxNode* pNode, FbxCluster* pCluster)
{
	for (unsigned int i = 0; i < mNodes.size(); i++)
	{
		if (mNodes[i]->SourceNode == pNode && mNodes[i]->SourceCluster == pCluster)
			return i;
	}

	float	InverseBindMatrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	float	LocalTransform[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

	FbxAMatrix transform;
	if (pCluster)
	{
		FbxAMatrix transform;
		pCluster->GetTransformMatrix(transform);
		FbxAMatrix transformLink;
		pCluster->GetTransformLinkMatrix(transformLink);
		FbxAMatrix InverseBindPose = transformLink.Inverse() * transform;

		for (int row = 0; row < 4; row++)
			for (int column = 0; column < 4; column++)
				InverseBindMatrix[row*4 + column] = (float)InverseBindPose.Get(row, column);
	}

	transform = pNode->EvaluateLocalTransform();
	for (int row = 0; row < 4; row++)
		for (int column = 0; column < 4; column++)
			LocalTransform[row*4 + column] = (float)transform.Get(row, column);

	NodeData* nd = new NodeData;
	nd->ID = mNodes.size();
	nd->SourceCluster = pCluster;
	nd->SourceNode = pNode;
	memcpy(nd->LocalTransform, LocalTransform, sizeof(float)*16);
	memcpy(nd->InverseBindMatrix, InverseBindMatrix, sizeof(float)*16);
	mNodes.push_back(nd);
	return nd->ID;
}
NodeData* FBXParser::GetNode(int pIndex)
{
	return mNodes[pIndex];
}
unsigned int FBXParser::FindIndex(FbxAnimLayer* pAnimLayer)
{
	for (unsigned int i = 0; i < mAnimationLayers.size(); i++)
	{
		if (mAnimationLayers[i]->Source == pAnimLayer)
			return i;
	}
	return -1;
}
MaterialData* FBXParser::GetMaterial(FbxSurfaceMaterial* pMaterial)
{
	for (int i = 0; i < mMaterials.size(); i++)
	{
		if (mMaterials[i]->Source == pMaterial)
			return mMaterials[i];
	}
	return NULL;
}
vector<AglConnectionPoint> FBXParser::GetConnectionPoints()
{
	return mConnectionPoints;
}