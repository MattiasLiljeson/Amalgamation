#include "AglWriter.h"

AglWriter::AglWriter(string p_path)
{
	m_path = p_path;
	m_header.nameID = 0;
	m_header.version = AGLVERSION;
	m_header.meshCount = 0;
	m_header.materialCount = 0;
	m_header.materialMappingCount = 0;
	m_header.nodeCount = 0;
	m_header.skeletonCount = 0;
	m_header.skeletonMappingCount = 0;
	m_header.nodeAnimationCount = 0;
	m_header.animationLayerCount = 0;
	m_header.animationCount = 0;
}
void AglWriter::write(AglScene* p_scene)
{
	AglSceneDesc d = p_scene->getSceneData();

	d.names.push_back("Agile");
	int nameIDindex = d.names.size()-1;

	int NameArrayLength = 0;
	for (unsigned int i = 0; i < d.names.size(); i++)
	{
		NameArrayLength += d.names[i].size();
	}
	NameArrayLength += d.names.size();

	m_header.nameID					= nameIDindex;
	m_header.version				= AGLVERSION;
	m_header.nameArrayLength		= NameArrayLength;
	m_header.meshCount				= d.meshes.size();
	m_header.materialCount			= d.materials.size();
	m_header.materialMappingCount	= d.materialMappings.size();
	m_header.nodeCount				= d.nodes.size();
	m_header.skeletonCount			= d.skeletons.size();
	m_header.skeletonMappingCount	= d.skeletonMappings.size();
	m_header.nodeAnimationCount		= d.nodeAnimations.size();
	m_header.animationLayerCount	= d.animationLayers.size();
	m_header.animationCount			= d.animations.size();
	m_header.looseBspCount			= d.bspTrees.size();
	m_header.SphereGridCount		= d.sphereGrids.size();

	ofstream file;

	file.open(m_path.c_str(), ios::out | ios::binary);

	//Write the header to the file
	char* buf = (char*)&m_header;
	file.write(buf, sizeof(AglTopHeader));


	//Write the name array to the file
	buf = new char[NameArrayLength];
	int ind = 0;
	for (unsigned int i = 0; i < d.names.size(); i++)
	{
		for (unsigned int j = 0; j < d.names[i].size(); j++)
		{
			buf[ind++] = d.names[i][j];
		}
		buf[ind++] = '\0';
	}
	file.write(buf, sizeof(char) * NameArrayLength);

	//Write all the mesh data to the file
	for (unsigned int i = 0; i < d.meshes.size(); i++)
	{
		//Write a mesh header to the file
		AglMeshHeader mheader = d.meshes[i]->getHeader();
		buf = (char*)&mheader;
		file.write(buf, sizeof(AglMeshHeader));

		//Write some data
		writeVertices(d.meshes[i], file);

		unsigned int* indices = d.meshes[i]->getIndices();
		buf = (char*)&indices[0];
		file.write(buf, sizeof(unsigned int)*mheader.indexCount);
	}

	//Write all the material data to the file
	vector<AglMaterial> materials;
	for (unsigned int i = 0; i < d.materials.size(); i++)
	{
		materials.push_back(*d.materials[i]);
	}

	if (d.materials.size() > 0)
	{
		buf = (char*)&materials[0];
		file.write(buf, sizeof(AglMaterial) * m_header.materialCount);
	}

	//Write all the material mapping data to the file
	if (d.materialMappings.size() > 0)
	{
		buf = (char*)&d.materialMappings[0];
		file.write(buf, sizeof(AglMaterialMapping) * m_header.materialMappingCount);
	}

	//Write all the node data to the file
	if (d.nodes.size() > 0)
	{
		buf = (char*)&d.nodes[0];
		file.write(buf, sizeof(AglNode) * m_header.nodeCount);
	}

	//Write all the skeleton data to the file
	for (unsigned int i = 0; i < d.skeletons.size(); i++)
	{
		//Write a skeleton header to the file
		AglSkeletonHeader sheader = d.skeletons[i]->getHeader();
		buf = (char*)&sheader;
		file.write(buf, sizeof(AglSkeletonHeader));

		AglJoint* sjoints = d.skeletons[i]->getJoints();
		buf = (char*)&sjoints[0];
		file.write(buf, sizeof(AglJoint) * sheader.jointCount);
	}

	for (unsigned int i = 0; i < d.skeletonMappings.size(); i++)
	{
		//Write a SkeletonMapping header to the file
		AglSkeletonMappingHeader smheader = d.skeletonMappings[i]->getHeader();
		buf = (char*)&smheader;
		file.write(buf, sizeof(AglSkeletonMappingHeader));

		AglSkeletonMappingVertex* smvs = d.skeletonMappings[i]->getVertices();
		buf = (char*)&smvs[0];
		file.write(buf, sizeof(AglSkeletonMappingVertex) * smheader.vertexCount);
	}

	for (unsigned int i = 0; i < d.nodeAnimations.size(); i++)
	{
		//Write a NodeAnimation header to the file
		AglNodeAnimationHeader nheader = d.nodeAnimations[i]->getHeader();
		buf = (char*)&nheader;
		file.write(buf, sizeof(AglNodeAnimationHeader));

		AglKeyFrame* frames = d.nodeAnimations[i]->getKeyFrames();
		buf = (char*)&frames[0];
		file.write(buf, sizeof(AglKeyFrame) * nheader.keyFrameCount);
	}

	for (unsigned int i = 0; i < d.animationLayers.size(); i++)
	{
		//Write an AnimationLayer header to the file
		AglAnimationLayerHeader lheader = d.animationLayers[i]->getHeader();
		buf = (char*)&lheader;
		file.write(buf, sizeof(AglAnimationLayerHeader));

		unsigned int* nodes = d.animationLayers[i]->getNodes();
		buf = (char*)&nodes[0];
		file.write(buf, sizeof(unsigned int) * lheader.nodeCount);
	}

	for (unsigned int i = 0; i < d.animations.size(); i++)
	{
		//Write an Animation header to the file
		AglAnimationHeader aheader = d.animations[i]->getHeader();
		buf = (char*)&aheader;
		file.write(buf, sizeof(AglAnimationHeader));

		unsigned int* layers = d.animations[i]->getLayers();
		buf = (char*)&layers[0];
		file.write(buf, sizeof(unsigned int) * aheader.layerCount);
	}
	for (unsigned int i = 0; i < d.bspTrees.size(); i++)
	{
		//Write a bsp tree header to the file
		AglLooseBspTreeHeader bspheader = d.bspTrees[i]->getHeader();
		buf = (char*)&bspheader;
		file.write(buf, sizeof(AglLooseBspTreeHeader));

		//Write the triangles of the bsp tree
		unsigned int* triangles = d.bspTrees[i]->getTriangles();
		buf = (char*)&triangles[0];
		file.write(buf, sizeof(unsigned int) * bspheader.triangleCount);

		//Write the nodes of the bsp tree
		AglBspNode* nodes = d.bspTrees[i]->getNodes();
		buf = (char*)&nodes[0];
		file.write(buf, sizeof(AglBspNode) * bspheader.nodeCount);
	}
	for (unsigned int i = 0; i < d.sphereGrids.size(); i++)
	{
		//Write a sphere grid header to the file
		AglInteriorSphereGridHeader gridheader = d.sphereGrids[i]->getHeader();
		buf = (char*)&gridheader;
		file.write(buf, sizeof(AglInteriorSphereGridHeader));

		//Write the spheres
		AglInteriorSphere* spheres = d.sphereGrids[i]->getSpheres();
		buf = (char*)&spheres[0];
		file.write(buf, sizeof(AglInteriorSphere) * gridheader.sphereCount);
	}

	file.close();
}
void AglWriter::writeVertices(AglMesh* p_mesh, ofstream& p_file)
{
	void* vertices = p_mesh->getVertices();
	AglMeshHeader header = p_mesh->getHeader();

	if (header.vertexFormat == AGL_VERTEX_FORMAT_PNT)
	{
		AglVertexSPNT* spnt = (AglVertexSPNT*)vertices;
		char* buf = (char*)&spnt[0];
		p_file.write(buf, sizeof(AglVertexSPNT)*header.vertexCount);
	}
	else if (header.vertexFormat == AGL_VERTEX_FORMAT_PNTTBN)
	{
		AglVertexSTBN* stbn = (AglVertexSTBN*)vertices;
		char* buf = (char*)&stbn[0];
		p_file.write(buf, sizeof(AglVertexSTBN)*header.vertexCount);
	}
}