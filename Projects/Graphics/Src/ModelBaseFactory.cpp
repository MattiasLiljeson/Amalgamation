#include "ModelBaseFactory.h"
#include "AglReader.h"



ModelResource* ModelBaseFactory::createModelDescription( const string& p_name, 
												 const string* p_path/*=NULL*/)
{
	ModelResource* model = NULL;
	// Check and read the file
	AglScene* scene = readScene(p_name,p_path);
	//
	if (scene)
	{ 
		vector<ModelResource*>* models = createAllModelData(scene,1);
		if ((*models)[0]!=NULL) model = (*models)[0];
	}
	// cleanup
	delete scene;

	return model;
}

vector<ModelResource*>* ModelBaseFactory::createModelDescriptions(const string& p_name,
																  const string* p_path/*=NULL*/)
{
	vector<ModelResource*>* models = NULL;
	// Check and read the file
	AglScene* scene = readScene(p_name,p_path);
	//
	if (scene)
	{ 
		models = createAllModelData(scene,scene->getMeshes().size());
	}
	// cleanup
	delete scene;

	return models;
}




#ifdef BLARGU
unsigned int GraphicsWrapper::createMeshFromRaw( const string& p_name, 
										 int p_numVertices, PNTTBVertex* p_vertices, 
										 int p_numIndices, DIndex* p_indices, 
										 Texture* p_texture/*=NULL*/ )
{
	// check if resource already exists
	unsigned int meshResultId = 0;
	int meshFoundId = m_meshManager->getResourceId(p_name);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
		Mesh* mesh = m_bufferFactory->createMeshFromPNTTBVerticesAndIndices( p_numVertices,
		p_vertices, p_numIndices, p_indices );

		meshResultId = registerMesh( p_name, mesh, p_texture ); // HACK: textures should be handled 
																// by index instead
	}
	else // the mesh already exists
	{
		meshResultId = static_cast<unsigned int>(meshFoundId);
	}
	return meshResultId;
}




#endif

AglScene* ModelBaseFactory::readScene(const string& p_name,
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

vector<ModelResource*>* ModelBaseFactory::createAllModelData( AglScene* p_scene, 
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
			model->meshHeader = &aglMeshHeader;
			model->name = p_scene->getName(i);

			readConnectionPoints(i,model,p_scene);
			readParticleSystems(i,model,p_scene);

			// Done
			models->push_back(model);
		}
	}
	return models;
}

void ModelBaseFactory::readConnectionPoints(unsigned int p_modelNumber,
											ModelResource* p_model, AglScene* p_scene)
{
	unsigned int connectionPoints = p_scene->getConnectionPointCount();
	for (unsigned int n=0;n<connectionPoints;n++)
	{
		AglConnectionPoint* cp = &p_scene->getConnectionPoint(n);
		if (cp->parentMesh == static_cast<int>(p_modelNumber))
		{
			p_model->connectionPoints->m_collection.push_back(cp->transform);
		}
	}
}

void ModelBaseFactory::readParticleSystems( unsigned int p_modelNumber, 
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
			p_model->particleSystems->m_collection.push_back(*ps);
		}
	}
}
