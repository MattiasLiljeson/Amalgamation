#include "ModelBaseUnmanagedFactory.h"
#include "AglReader.h"



ModelResource* ModelBaseUnmanagedFactory::createModelResource( const string& p_name, 
												 const string* p_path/*=NULL*/)
{
	ModelResource* model = NULL;
	// Check and read the file
	AglScene* scene = readScene(p_name,p_path);
	//
	if (scene)
	{ 
		vector<ModelResource*>* models = createAllModelData(scene,1);
		if ((*models)[0]!=NULL)
		{
			model = (*models)[0];
			readAndStoreEmpties(-1,model,scene); // read leftover empties
		}
	}
	else
	{
		model = getFallback();
	}
	// cleanup
	delete scene;

	return model;
}

vector<ModelResource*>* ModelBaseUnmanagedFactory::createModelResources( const string& p_name, 
														const string* p_path/*=NULL*/)
{
	vector<ModelResource*>* models = NULL;
	// Check and read the file
	AglScene* scene = readScene(p_name,p_path);
	//
	if (scene)
	{ 
		models = createAllModelData(scene,scene->getMeshes().size());
		if ((*models)[0]!=NULL)
		{
			ModelResource* model = (*models)[0];
			readAndStoreEmpties(-1,model,scene); // read leftover empties
		}
	}
	else
	{
		models = new vector<ModelResource*>();
		models->push_back(getFallback());
	}
	// cleanup
	delete scene;

	return models;
}

AglScene* ModelBaseUnmanagedFactory::readScene(const string& p_name,
							const string* p_path)
{
	string fullPath;
	if (p_path!=NULL) fullPath = *p_path;
	fullPath += p_name;
	// test file
	string fileChkMsg;
	if (!isFileOk(fullPath,fileChkMsg,__FILE__,__FUNCTION__,__LINE__))
		throw MeshLoadException(fileChkMsg);
	// read file and extract scene
	AglReader meshReader(fullPath.c_str());
	AglScene* aglScene = meshReader.getScene();
	return aglScene;
}

vector<ModelResource*>* ModelBaseUnmanagedFactory::createAllModelData( AglScene* p_scene, 
													 unsigned int p_numberOfModels/*=1*/ )
{
	vector<ModelResource*>* models = new vector<ModelResource*>;
	for (unsigned int i=0; i<p_numberOfModels; i++)
	{
		if (i<p_scene->getMeshes().size())
		{		
			ModelResource* model = new ModelResource();

			AglMesh* aglMesh = p_scene->getMeshes()[i];
			AglMeshHeader aglMeshHeader = aglMesh->getHeader();		
			// set
			model->meshHeader = aglMeshHeader;

			model->looseBspTree = createBspTree(aglMesh);

			readAndStoreEmpties((int)i,model,p_scene);
			readAndStoreParticleSystems(i,model,p_scene);

			// Done
			models->push_back(model);
		}
	}
	return models;
}

void ModelBaseUnmanagedFactory::readAndStoreEmpties( int p_modelNumber, ModelResource* p_model, AglScene* p_scene )
{
	unsigned int connectionPoints = p_scene->getConnectionPointCount();
	for (unsigned int n=0;n<connectionPoints;n++)
	{
		AglConnectionPoint* cp = &p_scene->getConnectionPoint(n);
		if (cp->parentMesh == p_modelNumber)
		{
			p_model->connectionPoints.m_collection.push_back(cp->transform);
		}
	}
}

void ModelBaseUnmanagedFactory::readAndStoreParticleSystems( unsigned int p_modelNumber, 
										    ModelResource* p_model, AglScene* p_scene )
{
	// ------------------
	// Particles
	// ------------------
	unsigned int particleSystems = p_scene->getParticleSystems().size();
	for (unsigned int n=0;n<particleSystems;n++)
	{
		AglParticleSystem* ps = p_scene->getParticleSystem(n);
		if (p_modelNumber==0) // add support for particle parent?
		{
			p_model->particleSystems.m_collection.push_back(*ps);
		}
	}
}

ModelResource* ModelBaseUnmanagedFactory::getFallback()
{
	// fallback mesh and texture
	ModelResource* model = new ModelResource();

	// Done
	return model;
}

AglLooseBspTree* ModelBaseUnmanagedFactory::createBspTree(AglMesh* p_mesh)
{
	//ANTON
	AglMeshHeader h = p_mesh->getHeader();
	AglVertexSTBN* v = (AglVertexSTBN*)p_mesh->getVertices();
	unsigned int* ind = p_mesh->getIndices();

	vector<AglVector3> vertices;
	vector<unsigned int> indices;
	for (int i = 0; i < h.vertexCount; i++)
	{
		vertices.push_back(v[i].position);
	}
	for (int i = 0; i < h.indexCount; i++)
	{
		indices.push_back(ind[i]);
	}
	AglLooseBspTreeConstructor constructor(h.id, vertices, indices);
	return constructor.createTree();
	//END ANTON
}
