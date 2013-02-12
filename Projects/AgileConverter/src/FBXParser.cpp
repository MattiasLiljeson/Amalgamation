#include "FBXParser.h"

FBXParser::FBXParser(string pPath)
{
	mData.OriginalPath = pPath;
}
FBXParser::~FBXParser()
{
	for (unsigned int i = 0; i < mData.Meshes.size(); i++)
	{
		delete[] mData.Meshes[i]->ControlIndices;
		delete mData.Meshes[i];
	}
}

void FBXParser::Parse()
{
	FbxManager* manager = FBXManager::GetInstance()->GetManager();
	FbxImporter* importer = FbxImporter::Create(manager,"");
    
	if(!importer->Initialize(mData.OriginalPath.c_str(), -1, manager->GetIOSettings())) 
	{
		cout << "Failed to initialize Importer";
        //cout << importer->GetLastErrorString() << "\n\n";
        return;
    }

    mScene = FbxScene::Create(manager, "");

	importer->Import(mScene);

	for (int i = 0; i < mScene->GetNodeCount(); i++)
	{
		const char* n = mScene->GetNode(i)->GetName();
		if (n[0] == 'I' && (n[1] == '_' || n[1] == '.'))
		{
			AglConnectionPoint cp;

			cp.parentMesh = -1;

			string name = mScene->GetNode(i)->GetName();
			FbxMatrix transform = mScene->GetNode(i)->EvaluateLocalTransform();
			for (int row = 0; row < 4; row++)
				for (int column = 0; column < 4; column++)
					cp.transform[row*4 + column] = (float)transform.Get(row, column);
			mData.CP.push_back(pair<AglConnectionPoint, string>(cp, name));
		}
		else
		{
			ParseNode(mScene->GetNode(i));
		}
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
				for (unsigned int i = 0; i < mData.Meshes.size(); i++)
				{
					if (node->GetParent() == mData.Meshes[i]->SourceNode)
					{
						cp.parentMesh = i;
						break;
					}
				}

				nodeNames.push_back(node->GetName()); 
				string name = node->GetName();
				FbxMatrix transform = node->EvaluateLocalTransform();
				for (int row = 0; row < 4; row++)
					for (int column = 0; column < 4; column++)
						cp.transform[row*4 + column] = (float)transform.Get(row, column);
				mData.CP.push_back(pair<AglConnectionPoint, string>(cp, name));
			}
		}
	}

	AnimationParser ap(this);
	ap.Parse(mData.Nodes);

	//Create mappings between geometry and other data
	for (unsigned int i = 0; i < mData.Meshes.size(); i++)
	{
		//Create mapping between mesh and skeleton
		FbxGeometry* geometry = mData.Meshes[i]->Source;
		int skins = geometry->GetDeformerCount(FbxDeformer::eSkin);
		for (int j = 0; j < skins; j++)
		{
			FbxSkin* s = (FbxSkin*)geometry->GetDeformer(j, FbxDeformer::eSkin);
			for (unsigned int k = 0; k < mData.Skeletons.size(); k++)
			{
				if (s == mData.Skeletons[k]->Source)
				{
					CreateMapping(mData.Meshes[i], mData.Skeletons[k]);
				}
			}
		}

		//Create Mapping between mesh and material
		FbxNode* node = mData.Meshes[i]->SourceNode;
		int materials = node->GetMaterialCount();
		for (int j = 0; j < materials; j++)
		{
			FbxSurfaceMaterial* mat = node->GetMaterial(j);
			for (unsigned int k = 0; k < mData.Materials.size(); k++)
			{
				if (mat == mData.Materials[k]->Source)
				{
					AglMaterialMapping* mapping = new AglMaterialMapping();
					mapping->materialID = k;
					mapping->meshID = i;
					mData.MaterialMappings.push_back(mapping);
				}
			}
		}
	}

	mScene->Destroy();
    importer->Destroy();
}
void FBXParser::ParseNode(FbxNode* pNode)
{
	static int skelCount = 0;
	FbxNodeAttribute* attr = pNode->GetNodeAttribute();
	if (attr)
	{
		FbxNodeAttribute::EType attrType = pNode->GetNodeAttribute()->GetAttributeType();
		if (attrType == FbxNodeAttribute::eMesh)
		{
			//Parse the mesh itself
			FbxMesh* mesh = (FbxMesh*)attr;

			const char* n = mesh->GetName();

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
	pMesh->ID = mData.Meshes.size();
	mData.Meshes.push_back(pMesh);
}
void FBXParser::AddSkeleton(SkeletonData* pSkeleton)
{
	pSkeleton->ID = mData.Skeletons.size();
	mData.Skeletons.push_back(pSkeleton);
}
void FBXParser::AddNodeAnimation(NodeAnimationData* pNodeAnimationData)
{
	pNodeAnimationData->ID = mData.NodeAnimations.size();
	mData.NodeAnimations.push_back(pNodeAnimationData);
}
void FBXParser::AddAnimationLayer(AnimationLayerData* pAnimationLayer)
{
	pAnimationLayer->ID = mData.AnimationLayers.size();
	mData.AnimationLayers.push_back(pAnimationLayer);
}
void FBXParser::AddAnimation(AnimationData* pAnimation)
{
	pAnimation->ID = mData.Animations.size();
	mData.Animations.push_back(pAnimation);
}
void FBXParser::AddMaterial(MaterialData* pMaterial)
{
	pMaterial->ID = mData.Materials.size();
	mData.Materials.push_back(pMaterial);
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
		if (mData.Nodes[pSkeleton->Joints[i]->NodeID]->SourceCluster)
		{
			FbxCluster* c	= mData.Nodes[pSkeleton->Joints[i]->NodeID]->SourceCluster;
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
	mData.MeshSkeletonMappings.push_back(m);
}

vector<MeshData*> FBXParser::GetMeshes()
{
	return mData.Meshes;
}
vector<NodeData*> FBXParser::GetNodes()
{
	return mData.Nodes;
}
vector<SkeletonData*> FBXParser::GetSkeletons()
{
	return mData.Skeletons;
}
vector<AglSkeletonMapping*> FBXParser::GetMeshSkeletonMappings()
{
	return mData.MeshSkeletonMappings;
}
vector<NodeAnimationData*> FBXParser::GetNodeAnimations()
{
	return mData.NodeAnimations;
}
vector<AnimationLayerData*> FBXParser::GetAnimationLayers()
{
	return mData.AnimationLayers;
}
vector<AnimationData*> FBXParser::GetAnimations()
{
	return mData.Animations;
}
vector<MaterialData*> FBXParser::GetMaterials()
{
	return mData.Materials;
}
vector<AglMaterialMapping*> FBXParser::GetMaterialMappings()
{
	return mData.MaterialMappings;
}

FbxScene* FBXParser::GetScene()
{
	return mScene;
}

unsigned int FBXParser::GetNodeID(FbxNode* pNode, FbxCluster* pCluster)
{
	for (unsigned int i = 0; i < mData.Nodes.size(); i++)
	{
		if (mData.Nodes[i]->SourceNode == pNode && mData.Nodes[i]->SourceCluster == pCluster)
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
	nd->ID = mData.Nodes.size();
	nd->SourceCluster = pCluster;
	nd->SourceNode = pNode;
	memcpy(nd->LocalTransform, LocalTransform, sizeof(float)*16);
	memcpy(nd->InverseBindMatrix, InverseBindMatrix, sizeof(float)*16);
	mData.Nodes.push_back(nd);
	return nd->ID;
}
NodeData* FBXParser::GetNode(int pIndex)
{
	return mData.Nodes[pIndex];
}
unsigned int FBXParser::FindIndex(FbxAnimLayer* pAnimLayer)
{
	for (unsigned int i = 0; i < mData.AnimationLayers.size(); i++)
	{
		if (mData.AnimationLayers[i]->Source == pAnimLayer)
			return i;
	}
	return -1;
}
MaterialData* FBXParser::GetMaterial(FbxSurfaceMaterial* pMaterial)
{
	for (int i = 0; i < mData.Materials.size(); i++)
	{
		if (mData.Materials[i]->Source == pMaterial)
			return mData.Materials[i];
	}
	return NULL;
}
vector<pair<AglConnectionPoint, string>> FBXParser::GetConnectionPoints()
{
	return mData.CP;
}