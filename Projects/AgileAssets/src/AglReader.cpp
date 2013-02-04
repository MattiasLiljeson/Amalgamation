#include "AglReader.h"

AglReader::AglReader(const char* p_path)
{
	m_scene = new AglScene();
	AglSceneDesc desc;

	m_file.open(p_path, ios::in | ios::binary);

	//Read the top header
	m_file.read ((char*)&m_topHeader, sizeof(AglTopHeader));

	//Read the names array
	desc.names = readNameArray();

	//Read the meshes
	for (int i = 0; i < m_topHeader.meshCount; i++)
	{
		desc.meshes.push_back(readMesh());
	}
	//Read the materials
	for (int i = 0; i < m_topHeader.materialCount; i++)
	{
		desc.materials.push_back(readMaterial());
	}
	for (int i = 0; i < m_topHeader.materialMappingCount; i++)
	{
		desc.materialMappings.push_back(readMaterialMapping());
	}
	//Read the nodes
	for (int i = 0; i < m_topHeader.nodeCount; i++)
	{
		desc.nodes.push_back(readNode());
	}
	//Read the Skeletons
	for (int i = 0; i < m_topHeader.skeletonCount; i++)
	{
		desc.skeletons.push_back(readSkeleton());
	}
	for (int i = 0; i < m_topHeader.skeletonMappingCount; i++)
	{
		desc.skeletonMappings.push_back(readSkeletonMapping());
	}
	for (int i = 0; i < m_topHeader.nodeAnimationCount; i++)
	{
		desc.nodeAnimations.push_back(readNodeAnimation());
	}
	for (int i = 0; i < m_topHeader.animationLayerCount; i++)
	{
		desc.animationLayers.push_back(readAnimationLayer());
	}
	for (int i = 0; i < m_topHeader.animationCount; i++)
	{
		desc.animations.push_back(readAnimation());
	}
	for (int i = 0; i < m_topHeader.looseBspCount; i++)
	{
		desc.bspTrees.push_back(readBspTree());
	}
	for (int i = 0; i < m_topHeader.SphereGridCount; i++)
	{
		desc.sphereGrids.push_back(readSphereGrid());
	}
	for (int i = 0; i < m_topHeader.connectionPointCount; i++)
	{
		desc.connectionPoints.push_back(readConnectionPoint());
	}
	for (int i = 0; i < m_topHeader.particleSystemCount; i++)
	{
		AglParticleSystem* ps = new AglParticleSystem(readParticleSystem());
		desc.particleSystems.push_back(ps);
	}

	desc.coordinateSystem = m_topHeader.coordinateSystem;

	m_scene->init(desc);

	m_file.close();
}
AglReader::~AglReader()
{
}
vector<string> AglReader::readNameArray()
{
	vector<string> names;
	char* buf = new char[m_topHeader.nameArrayLength];
	m_file.read (buf, sizeof(char) * m_topHeader.nameArrayLength);
	int ind = 0;
	string name = "";
	while (ind < m_topHeader.nameArrayLength)
	{
		if (buf[ind] == '\0')
		{
			names.push_back(name);
			name = "";
		}
		else
		{
			name += buf[ind];
		}
		ind++;
	}
	delete[] buf;
	return names;
}

AglMesh* AglReader::readMesh()
{
	AglMeshHeader header;
	m_file.read ((char*)&header, sizeof(AglMeshHeader));

	void* vertices;
	if (header.vertexFormat == AGL_VERTEX_FORMAT_PNT)
	{
		vertices = readVertices<AglVertexSPNT>(header.vertexCount);
	}
	else if (header.vertexFormat == AGL_VERTEX_FORMAT_PNTTBN)
	{
		vertices = readVertices<AglVertexSTBN>(header.vertexCount);
	}
	else
	{
		//Unsupported Vertex Format
	}

	unsigned int* indices = new unsigned int[header.indexCount];
	m_file.read((char*)&indices[0], sizeof(unsigned int) * header.indexCount);

	AglMesh* mesh = new AglMesh(header, vertices, indices);
	return mesh;
}

template <typename T>
T* AglReader::readVertices(int p_count)
{
	T* vertices = new T[p_count];
	m_file.read ((char*)vertices, sizeof (T) * p_count);
	return vertices;
}

AglMaterial* AglReader::readMaterial()
{
	AglMaterial* material = new AglMaterial();
	m_file.read((char*)material, sizeof(AglMaterial));
	return material;
}
AglMaterialMapping AglReader::readMaterialMapping()
{
	AglMaterialMapping mm;
	m_file.read((char*)&mm, sizeof(AglMaterialMapping));
	return mm;
}
AglNode AglReader::readNode()
{
	AglNode node;
	m_file.read((char*)&node, sizeof(AglNode));
	return node;
}
AglSkeleton* AglReader::readSkeleton()
{
	AglSkeletonHeader header;
	m_file.read((char*)&header, sizeof(AglSkeletonHeader));

	AglJoint* joints = new AglJoint[header.jointCount];
	m_file.read((char*)&joints[0], sizeof(AglJoint)*header.jointCount);
	return new AglSkeleton(header, joints, m_scene);
}
AglSkeletonMapping* AglReader::readSkeletonMapping()
{
	AglSkeletonMappingHeader header;
	m_file.read((char*)&header, sizeof(AglSkeletonMappingHeader));

	AglSkeletonMappingVertex* smv = new AglSkeletonMappingVertex[header.vertexCount];
	m_file.read((char*)&smv[0], sizeof(AglSkeletonMappingVertex)*header.vertexCount);
	return new AglSkeletonMapping(header, smv);
}
AglNodeAnimation* AglReader::readNodeAnimation()
{
	AglNodeAnimationHeader header;
	m_file.read((char*)&header, sizeof(AglNodeAnimationHeader));

	AglKeyFrame* frames = new AglKeyFrame[header.keyFrameCount];
	m_file.read((char*)&frames[0], sizeof(AglKeyFrame)*header.keyFrameCount);
	return new AglNodeAnimation(header, frames, m_scene);
}
AglAnimationLayer* AglReader::readAnimationLayer()
{
	AglAnimationLayerHeader header;
	m_file.read((char*)&header, sizeof(AglAnimationLayerHeader));

	unsigned int* nodes = new unsigned int[header.nodeCount];
	m_file.read((char*)&nodes[0], sizeof(unsigned int)*header.nodeCount);
	return new AglAnimationLayer(header, nodes, m_scene);
}
AglAnimation* AglReader::readAnimation()
{
	AglAnimationHeader header;
	m_file.read((char*)&header, sizeof(AglAnimationHeader));

	unsigned int* layers = new unsigned int[header.layerCount];
	m_file.read((char*)&layers[0], sizeof(unsigned int)*header.layerCount);
	return new AglAnimation(header, layers, m_scene);
}
AglLooseBspTree* AglReader::readBspTree()
{
	AglLooseBspTreeHeader header;
	m_file.read((char*)&header, sizeof(AglLooseBspTreeHeader));

	unsigned int* triangles = new unsigned int[header.triangleCount];
	m_file.read((char*)&triangles[0], sizeof(unsigned int)*header.triangleCount);

	AglVector3* triangles2 = new AglVector3[header.triangleCount*3];
	m_file.read((char*)&triangles2[0], sizeof(AglVector3)*header.triangleCount*3);

	AglBspNode* nodes = new AglBspNode[header.nodeCount];
	m_file.read((char*)&nodes[0], sizeof(AglBspNode)*header.nodeCount);

	return new AglLooseBspTree(header, triangles, triangles2, nodes);
}
AglInteriorSphereGrid* AglReader::readSphereGrid()
{
	AglInteriorSphereGridHeader header;
	m_file.read((char*)&header, sizeof(AglInteriorSphereGridHeader));

	AglInteriorSphere* spheres = new AglInteriorSphere[header.sphereCount];
	m_file.read((char*)&spheres[0], sizeof(AglInteriorSphere)*header.sphereCount);

	return new AglInteriorSphereGrid(header, spheres);
}

AglConnectionPoint AglReader::readConnectionPoint()
{
	AglConnectionPoint cp;
	m_file.read((char*)&cp, sizeof(AglConnectionPoint));
	return cp;
}

AglParticleSystemHeader AglReader::readParticleSystem()
{
	AglParticleSystemHeader ps;
	m_file.read((char*)&ps, sizeof(AglParticleSystemHeader));
	return ps;
}

AglScene* AglReader::getScene()
{
	return m_scene;
}