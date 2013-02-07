#include "ModelBaseFactory.h"
#include <AglReader.h>
#include <AglScene.h>
#include "MeshNameScriptParser.h"


ModelBaseFactory::ModelBaseFactory()
{
	m_modelResourceCache = new ResourceManager<ModelResourceCollection>;
}

ModelBaseFactory::~ModelBaseFactory()
{
	delete m_modelResourceCache;
}


// ModelResource* ModelBaseFactory::createModelResource_DEPRECATED( const string& p_name, 
// 												 const string* p_path/*=NULL*/)
// {
// 	ModelResource* model = NULL;
// 	// Check and read the file
// 	vector<ModelResource*>* models = createModelResources( p_name, p_path);
// 	model = (*models)[0];
// 	return model;
// }

vector<ModelResource*>* ModelBaseFactory::createModelResources( const string& p_name, 
														const string* p_path)
{
	InstanceInstruction currentInstance={p_name,AglMatrix::identityMatrix()};
	//
	vector<ModelResource*>* models = NULL;
	vector<InstanceInstruction>* instanceInstructions = new vector<InstanceInstruction>();
	int instanceCount = 0;
	// Check and read the file
	do 
	{	
		if (!instanceInstructions->empty()) instanceInstructions->pop_back();

		int modelFoundId = m_modelResourceCache->getResourceId(currentInstance.filename);
		// ---------------------
		// New mesh
		// ---------------------
		if (modelFoundId==-1)  // if it does not exist, create new
		{
			AglScene* scene = readScene(currentInstance.filename,p_path);
			//
			if (scene)
			{ 
				// DEBUGWARNING(( ("Loading meshes from "+currentInstance.filename+" instance="+toString(instanceCount)).c_str() ));
				if (instanceCount==0) 
				{
					// just a normal mesh, just copy resource instructions
					models = createAllModelData(&currentInstance,
						scene,
						instanceInstructions);		
					// read leftover empties
					if ((*models)[0]!=NULL)
					{
						ModelResource* model = (*models)[0];
						SourceData source={scene,NULL,NULL,-1,string("")};
						readAndStoreEmpties(source,model,currentInstance.transform,
							&currentInstance,instanceInstructions); 
					}
				}
				else
				{
					// an instance needs to add(instead of copy) to original collection from 
					// the instance's read collection.
					// it also needs to copy the model resource data
					vector<ModelResource*>* prefetched = createAllModelData(&currentInstance,
						scene,
						instanceInstructions);
					// read leftover empties
					ModelResource* root = (*prefetched)[0];
					if (root!=NULL)
					{
						SourceData source={scene,NULL,NULL,-1,string("")};
						readAndStoreEmpties(source,root,currentInstance.transform,
											&currentInstance,instanceInstructions); 
					}
					//
					ModelResourceCollection* modelresourceCollection = m_modelResourceCache->getResource(currentInstance.filename);
					unsigned int start = modelresourceCollection->rootIndex+1;
					unsigned int size = prefetched->size();
					for (unsigned int n=start;n<size;n++)
					{
						ModelResource* model = new ModelResource( *(*prefetched)[n] );
						// mesh transform
						model->transform *= currentInstance.transform;
						// 
						models->push_back(model);
					}
				}

			}
			else
			{
				models->push_back(getFallback());
			}
			// cleanup
			delete scene;
		}
		else // the mesh already exists
		{
		// ---------------------
		// Existing mesh
		// ---------------------
			if (instanceCount==0) 
			{
				// just a normal mesh, just copy resource instructions
				models = &m_modelResourceCache->getResource(currentInstance.filename)->collection;
			}
			else					
			{
				// an instance needs to add to original collection from  the instance's collection
				// it also needs to copy the model resource data
				ModelResourceCollection* modelresourceCollection = m_modelResourceCache->getResource(currentInstance.filename);
				vector<ModelResource*>* prefetched = &modelresourceCollection->collection;

				unsigned int root = modelresourceCollection->rootIndex;
				unsigned int size = prefetched->size();
				for (unsigned int n=0;n<size;n++)
				{
					ModelResource* model = new ModelResource( *(*prefetched)[n] );
					// mesh transform
					model->transform *= currentInstance.transform;
					AglMatrix* base= &(model->transform);
					// instances
					for (unsigned int x=0;x<model->instances.size();x++)
					{
						InstanceInstruction instruction = model->instances[x];
						instruction.transform *= currentInstance.transform;
						instanceInstructions->push_back(instruction);
					}
					// 
					if (n!=root) 
						models->push_back(model);
					else
						delete model;
				}
			}
		}

		// read and prepare next instance if file had any instances specified
		if (!instanceInstructions->empty())
		{
			currentInstance = instanceInstructions->back();
		}
		instanceCount++;

	} while (!instanceInstructions->empty());


	delete instanceInstructions;

	return models;
}

AglScene* ModelBaseFactory::readScene(const string& p_name, const string* p_path)
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

vector<ModelResource*>* ModelBaseFactory::createAllModelData( 
	const InstanceInstruction* p_instanceData, 
	AglScene* p_scene, 
	vector<InstanceInstruction>* p_outInstanceInstructions)
{
	ModelResourceCollection* models = new ModelResourceCollection();
	// if several models were found, or none, add a root entity
	unsigned int numberOfModels = p_scene->getMeshes().size();
	if (numberOfModels>1 || numberOfModels==0)
	{
		ModelResource* mr = new ModelResource(p_instanceData->filename+"-ROOT");

		//Neccessary Oriented bounding box for collision detection - ADDED BY ANTON
		mr->meshHeader.minimumOBB = p_scene->getSceneOBB();
		mr->meshHeader.transform = AglMatrix::identityMatrix();

		models->collection.push_back(mr);
		models->rootIndex=models->collection.size()-1;
	}
	// check all models
	for (unsigned int i=0; i<numberOfModels; i++)
	{	
		AglMesh* aglMesh = p_scene->getMeshes()[i];
		AglMeshHeader aglMeshHeader = aglMesh->getHeader();	
		// parse mesh name
		string meshName = p_scene->getName(aglMeshHeader.nameID);
		pair<MeshNameScriptParser::Data,MeshNameScriptParser::Token> parsedAction;
		parsedAction = MeshNameScriptParser::parse(meshName);
		// DEBUGWARNING(( ("Creating mesh "+meshName).c_str() ));
		// Actions based on parsed name
		switch (parsedAction.second) 
		{
		case MeshNameScriptParser::INSTANTIATE: // instantiate
			{
				if (p_outInstanceInstructions!=NULL)
				{
					InstanceInstruction inst = {parsedAction.first.instanceSpecFilename,
						aglMeshHeader.transform};

					DEBUGWARNING(((p_instanceData->filename+": Found mesh with instancing instruction. Conversion error?").c_str()));
					// Not possible yet-> p_model->instances.push_back(inst);
					p_outInstanceInstructions->push_back(inst);
				}

				break;
			}

		case MeshNameScriptParser::MESH: // normal mesh
		default:				
			{
				// DEBUGWARNING(( string("Normal mesh").c_str() ));
				SourceData source={p_scene,aglMesh,&aglMeshHeader,
					i,
					parsedAction.first.name};
				//
				createAndAddModel(models, 
					p_instanceData, 
					source, 
					p_outInstanceInstructions);
				break;
			}
		}

	}
	m_modelResourceCache->addResource(p_instanceData->filename,models); // register collection in cache

	return &models->collection;
}


void ModelBaseFactory::createAndAddModel( ModelResourceCollection* p_modelCollection, 
										 const InstanceInstruction* p_instanceData, 
										 ModelBaseFactory::SourceData& p_source,
										 vector<InstanceInstruction>* p_outInstanceInstructions)
{
	// set
	ModelResource* model = new ModelResource();
	model->meshHeader = *(p_source.meshHeader);
	p_source.nameSuffix = "_"+p_source.nameSuffix;
	if (p_source.modelNumber==0)
	{
		p_source.nameSuffix="";
	}
	// Mesh data
	unsigned int meshResultId = createMeshData( p_source,p_instanceData );
	// store in model
	model->name = p_instanceData->filename+p_source.nameSuffix;
	model->meshId = static_cast<int>(meshResultId);
	model->transform = p_source.meshHeader->transform;

	// other model creation data
	readAndStoreEmpties(p_source,
		model,
		model->transform,
		p_instanceData,
		p_outInstanceInstructions);
	readAndStoreParticleSystems(p_source,model);


	//Store bsp tree
	vector<AglLooseBspTree*> trees = p_source.scene->getBspTrees();
	for (unsigned int i = 0; i < trees.size(); i++)
	{
		if (trees[i]->getHeader().targetMesh == p_source.aglMesh->getHeader().id)
		{
			model->looseBspTree = trees[i]->clone();
		}
	}


	// Done
	p_modelCollection->collection.push_back(model); // register in collection
}


unsigned int ModelBaseFactory::createMeshData( ModelBaseFactory::SourceData& p_source,
											  const InstanceInstruction* p_instanceData)
{
	unsigned int meshResultId=-1;
	return meshResultId;
}



void ModelBaseFactory::readAndStoreEmpties( SourceData& p_source, 
										   ModelResource* p_model, 
										   AglMatrix& p_offset,
										   const InstanceInstruction* p_instanceData, 
										   vector<InstanceInstruction>* p_outInstanceInstructions)
{
	unsigned int connectionPoints = p_source.scene->getConnectionPointCount();
	for (unsigned int n=0;n<connectionPoints;n++)
	{
		AglConnectionPoint* cp = &p_source.scene->getConnectionPoint(n);
		// parse name
		string name = p_source.scene->getName(cp->nameID);
		pair<MeshNameScriptParser::Data,MeshNameScriptParser::Token> parsedAction;
		parsedAction = MeshNameScriptParser::parse(name);
		// Actions based on parsed name
		switch (parsedAction.second) 
		{
		case MeshNameScriptParser::INSTANTIATE: // instantiate
			{
				if (cp->parentMesh == p_source.modelNumber) // handle global and local call the same
				{
					InstanceInstruction inst = {parsedAction.first.instanceSpecFilename,
						cp->transform};
					// DEBUGWARNING(( ("Found instance "+parsedAction.first.filename).c_str() ));
					p_model->instances.push_back(inst);
					inst.transform *= p_offset;
					p_outInstanceInstructions->push_back(inst);
				}
				break;
			}

		case MeshNameScriptParser::SPAWNPOINT: // spawn point		
			{
				bool isOk = false;
				if (p_source.modelNumber!=-1) // call from parent
					isOk = (cp->parentMesh == p_source.modelNumber);
				else // call from global
					isOk = (cp->parentMesh == -1 && p_model!=NULL);

				if (isOk)
				{
					pair<string,AglMatrix> sp(parsedAction.first.spawnSpecName,
						cp->transform*p_offset);
					p_model->spawnPoints.m_collection.push_back(sp);
				}

				break;
			}
			
		case MeshNameScriptParser::CONNECTIONPOINT: // normal cp			
			{
				bool isOk = false;
				if (p_source.modelNumber!=-1) // call from parent
					isOk = (cp->parentMesh == p_source.modelNumber);
				else // call from global
					isOk = (cp->parentMesh == -1 && p_model!=NULL);

				if (isOk)
					p_model->connectionPoints.m_collection.push_back(cp->transform*p_offset);

				break;
			}
			
		case MeshNameScriptParser::POINTLIGHT: // lights
		case MeshNameScriptParser::DIRLIGHT:
		case MeshNameScriptParser::SPOTLIGHT:	
			{
				bool isOk = false;
				if (p_source.modelNumber!=-1) // call from parent
					isOk = (cp->parentMesh == p_source.modelNumber);
				else // call from global
					isOk = (cp->parentMesh == -1 && p_model!=NULL);

				if (isOk)
				{
					LightCreationData ld = parsedAction.first.lightSpec;
					ld.transform = cp->transform*p_offset;
					p_model->lightCollection.m_collection.push_back(ld);
				}

				break;
			}


			
		default:				
			{
				break;
			}			
			
		}
	}
}

void ModelBaseFactory::readAndStoreParticleSystems( SourceData& p_source, 
													ModelResource* p_model)
{
	// ------------------
	// Particles
	// ------------------
	unsigned int particleSystems = p_source.scene->getParticleSystems().size();
	for (unsigned int n=0;n<particleSystems;n++)
	{
		AglParticleSystem* ps = p_source.scene->getParticleSystem(n);
		ParticleSystemInstruction psIntr;
		psIntr.particleSystem = *ps;
		psIntr.textureFileName = p_source.scene->getName(ps->getHeader().textureNameIndex,
														 true);
		if (p_source.modelNumber==0) // add support for particle parent?
		{
			p_model->particleSystems.m_collection.push_back(psIntr);
		}
	}
}

ModelResource* ModelBaseFactory::getFallback()
{
	// fallback mesh and texture
	ModelResource* model = new ModelResource();

	// Done
	return model;
}

AglLooseBspTree* ModelBaseFactory::createBspTree(AglMesh* p_mesh)
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

