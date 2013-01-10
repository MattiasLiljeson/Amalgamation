#include "AGLExporter.h"

AGLExporter::AGLExporter(string pPath)
{
	mPath = new char[pPath.length()+1];
	strcpy(mPath, pPath.c_str());
	mScene = new AglScene();
}
AGLExporter::~AGLExporter()
{
	delete[] mPath;
	delete mScene;
	//for (unsigned int i = 0; i < mMeshes.size(); i++)
		//delete mMeshes[i];
}
void AGLExporter::AddMesh(MeshData* pData)
{
	AglMeshHeader h;
	h.id = pData->ID;
	h.nameID = mScene->addName(pData->Name);
	h.indexCount = pData->IndicesCount;
	h.vertexCount = pData->VertexCount;
	h.vertexFormat = pData->VertexFormat;

	//Write a loose bsp tree for the mesh
	vector<AglVector3> verts;
	AglVertexSTBN* oldV = (AglVertexSTBN*)pData->Vertices;
	for (int j = 0; j < h.vertexCount; j++)
	{
		verts.push_back(oldV[j].position);
	}
	vector<unsigned int> ind;
	for (int j = 0; j < h.indexCount; j++)
	{
		ind.push_back(pData->Indices[j]);
	}

	cout << "Creating bounding volumes for mesh " << h.id << endl;
	//Would be optimal with hulls rather than a generic mesh
	h.minimumOBB = AglOBB::constructMinimum(verts, ind);
	h.boundingSphere = AglBoundingSphere::minimumBoundingSphere(h.minimumOBB.getCorners());
	cout << "Bounding volumes created for mesh " << h.id << endl << endl;

	//AglInteriorSphereGrid* spheregrid = new AglInteriorSphereGrid(3, verts, ind, h.id);

	//mScene->addSphereGrid(spheregrid);

	//AglLooseBspTreeConstructor treeConst(h.id, verts, ind);
	//mScene->addBspTree(treeConst.createTree());

	AglMesh* m = new AglMesh(h, pData->Vertices, pData->Indices);
	mMeshes.push_back(m);
	mScene->addMesh(m);
}
void AGLExporter::AddNode(NodeData* pData)
{
	AglNode n;
	n.id = pData->ID;
	for (int i = 0; i < 16; i++)
	{
		n.inverseBindMatrix[i] = pData->InverseBindMatrix[i];
		n.localTransform[i] = pData->LocalTransform[i];
	}
	mNodes.push_back(n);
	mScene->addNode(n);
}
void AGLExporter::AddSkeleton(SkeletonData* pData)
{
	AglSkeletonHeader h;
	h.jointCount = pData->Joints.size();
	h.nameID = mScene->addName(pData->Name);
	h.baseTransform = AglMatrix::identityMatrix();

	AglJoint* joints = new AglJoint[pData->Joints.size()];
	for (unsigned int i = 0; i < pData->Joints.size(); i++)
	{
		joints[i].id = pData->Joints[i]->ID;
		joints[i].nodeID = pData->Joints[i]->NodeID;
		joints[i].parentIndex = pData->Joints[i]->Parent;
	}
	mSkeletons.push_back(new AglSkeleton(h, joints, NULL));
	mScene->addSkeleton(mSkeletons.back());
}
void AGLExporter::AddMeshSkeletonMapping(AglSkeletonMapping* pData)
{
	mMeshSkeletonMappings.push_back(pData);
	mScene->addSkeletonMapping(pData);
}
void AGLExporter::AddNodeAnimation(NodeAnimationData* pData)
{
	AglNodeAnimationHeader h;
	h.id = pData->ID;
	h.keyFrameCount = pData->KeyFrames.size();
	h.targetNode = pData->NodeID;

	AglKeyFrame* frames = new AglKeyFrame[h.keyFrameCount];
	for (unsigned int i = 0; i < h.keyFrameCount; i++)
	{
		frames[i].time = pData->KeyFrames[i].time;
		frames[i].transform = pData->KeyFrames[i].transform;
	}
	AglNodeAnimation* na = new AglNodeAnimation(h, frames, NULL);
	mNodeAnimations.push_back(na);
	mScene->addNodeAnimation(na);
}
void AGLExporter::AddAnimationLayer(AnimationLayerData* pData)
{
	AglAnimationLayerHeader h;
	h.id = pData->ID;
	h.nodeCount = pData->NodeAnimations.size();
	unsigned int* nodes = new unsigned int[h.nodeCount];
	for (unsigned int i = 0; i < h.nodeCount; i++)
	{
		nodes[i] = pData->NodeAnimations[i];
	}
	AglAnimationLayer* l = new AglAnimationLayer(h, nodes, NULL);
	mAnimationLayers.push_back(l);
	mScene->addAnimationLayer(l);
}
void AGLExporter::AddAnimation(AnimationData* pData)
{
	AglAnimationHeader h;
	h.id = pData->ID;
	h.layerCount = pData->Layers.size();
	unsigned int* layers = new unsigned int[h.layerCount];
	for (unsigned int i = 0; i < h.layerCount; i++)
	{
		layers[i] = pData->Layers[i];
	}
	AglAnimation* a = new AglAnimation(h, layers, NULL);
	mAnimations.push_back(a);
	mScene->addAnimation(a);
}
void AGLExporter::AddMaterial(MaterialData* pData)
{
	AglMaterial* mat = new AglMaterial;
	mat->id = pData->ID;
	for (int i = 0; i < 3; i++)
	{
		mat->ambient[i] = pData->Ambient[i];
		mat->diffuse[i] = pData->Diffuse[i];
		mat->specular[i] = pData->Specular[i];
		mat->emissive[i] = pData->Emissive[i];
	}
	mat->opacity = pData->Opacity;
	mat->reflectivity = pData->Reflectivity;
	mat->shininess = pData->Shininess;
	if (pData->DiffuseTexture != "")
		mat->diffuseTextureNameIndex = mScene->addName(pData->DiffuseTexture);
	else
		mat->diffuseTextureNameIndex = -1;

	if (pData->SpecularTexture != "")
		mat->specularTextureNameIndex = mScene->addName(pData->SpecularTexture);
	else
		mat->specularTextureNameIndex = -1;

	mat->normalTextureNameIndex = -1;
	mat->glowTextureNameIndex = -1;
	mat->nameID = mScene->addName("");

	mMaterials.push_back(mat);
	mScene->addMaterial(mat);
}
void AGLExporter::AddMaterialMapping(AglMaterialMapping* pData)
{
	AglMaterialMapping m;
	m.materialID = pData->materialID;
	m.meshID = pData->meshID;
	mMaterialMappings.push_back(m);
	mScene->addMaterialMapping(m);
}
void AGLExporter::AddConnectionPoint(pair<AglConnectionPoint, string> pCP)
{
	pCP.first.nameID = mScene->addName(pCP.second);
	mScene->addConnectionPoint(pCP.first);
}

void AGLExporter::Write()
{
	//Add a default material to all meshes before proceeding
	AglMaterial* mat = new AglMaterial();
	mat->id = mMaterials.size();
	mat->nameID = mScene->addName("");
	mMaterials.push_back(mat);
	mScene->addMaterial(mat);

	for (int i = 0; i < mMeshes.size(); i++)
	{
		AglMaterialMapping m;
		m.materialID = mMaterials.size()-1;
		m.meshID = i;
		mMaterialMappings.push_back(m);
		mScene->addMaterialMapping(m);
	}

	AglWriter writer(mPath);
	writer.write(mScene);
}