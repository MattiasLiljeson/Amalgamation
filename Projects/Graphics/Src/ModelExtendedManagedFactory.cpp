#include "ModelExtendedManagedFactory.h"
#include <AglScene.h>
#include <Globals.h>
#include "MaterialInfo.h"
#include "Mesh.h"
#include "MeshNameScriptParser.h"

const string& ModelExtendedManagedFactory::m_primitiveBoxName="P_box";
const string& ModelExtendedManagedFactory::m_primitiveSphereName="P_sphere";

ModelExtendedManagedFactory::ModelExtendedManagedFactory(ID3D11Device* p_device,BufferFactory* p_bufferFactory, 
	ResourceManager<Mesh>* p_resourceManager,
	TextureFactory* p_textureFactory)
{
	m_device = p_device;
	m_bufferFactory = p_bufferFactory;
	m_meshManager = p_resourceManager;
	m_textureFactory = p_textureFactory;
	m_modelResourceCache = new ResourceManager<ModelResourceCollection>;
}

ModelExtendedManagedFactory::~ModelExtendedManagedFactory()
{
	delete m_modelResourceCache;
}

ModelResource* ModelExtendedManagedFactory::createModelResource( const string& p_name, 
													 const string* p_path/*=NULL*/)
{
	ModelResource* model = NULL;
	// check if resource already exists
	// unsigned int meshResultId = 0;
	int meshFoundId = m_meshManager->getResourceId(p_name);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
		if (p_name == m_primitiveBoxName)
		{
			model = getBox();
		}
		else if (p_name == m_primitiveSphereName)
		{
			model = getSphere();
		}
		else
		{
			// Check and read the file
			AglScene* scene = readScene(p_name,p_path);
			//
			if (scene)
			{ 
				vector<ModelResource*>* models = createAllModelData(p_name,scene,1,NULL);
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
		}
	}
	else // the mesh already exists
	{
		model = m_modelResourceCache->getResource(p_name)->collection[0];
	}

	return model;
}

vector<ModelResource*>* ModelExtendedManagedFactory::createModelResources( const string& p_name, 
															   const string* p_path/*=NULL*/)
{
	vector<ModelResource*>* models = NULL;
	// Check and read the file
	int meshFoundId = m_meshManager->getResourceId(p_name);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
		AglScene* scene = readScene(p_name,p_path);
		//
		if (scene)
		{ 
			models = createAllModelData(p_name,scene,scene->getMeshes().size());
			if ((*models)[0]!=NULL)
			{
				ModelResource* model = (*models)[0];
				readAndStoreEmpties(-1,model,scene); // read leftover empties
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
		models = &m_modelResourceCache->getResource(p_name)->collection;
	}

	return models;
}


vector<ModelResource*>* ModelExtendedManagedFactory::createAllModelData( const string& p_name,  
																  AglScene* p_scene, 
																  unsigned int p_numberOfModels,
																  vector<InstanceInstr>* p_outInstanceInstructions)
{
	p_numberOfModels = max(p_numberOfModels,1);
	ModelResourceCollection* models = new ModelResourceCollection();
	for (unsigned int i=0; i<p_numberOfModels; i++)
	{
		if (i<p_scene->getMeshes().size())
		{		
			AglMesh* aglMesh = p_scene->getMeshes()[i];
			AglMeshHeader aglMeshHeader = aglMesh->getHeader();	
			// parse mesh name
			string meshName = p_scene->getName(aglMeshHeader.nameID);
			pair<MeshNameScriptParser::Data,MeshNameScriptParser::Token> parsedAction;
			parsedAction = MeshNameScriptParser::parse(meshName);
			// Actions based on parsed name
			switch (parsedAction.second) 
			{
			case MeshNameScriptParser::INSTANTIATE: // instantiate
				{
					if (p_outInstanceInstructions!=NULL)
					{
						InstanceInstr inst = {parsedAction.first.filename,
		/* Retrieve transform here! ---> */	  AglMatrix::identityMatrix()};
						p_outInstanceInstructions->push_back(inst);
					}

					break;
				}

			case MeshNameScriptParser::MESH: // normal mesh
			default:				
				{
					createAndAddModel(models, i, p_name+parsedAction.first.name, 
						p_scene, aglMesh, &aglMeshHeader);
					break;
				}
			}

		}
	}
	m_modelResourceCache->addResource(p_name,models); // register collection in cache

	return &models->collection;
}

void ModelExtendedManagedFactory::createAndAddModel( ModelResourceCollection* p_modelCollection, 
													unsigned int p_modelNumber, 
													const string& p_name, 
													AglScene* p_scene, AglMesh* p_aglMesh,
													AglMeshHeader* p_meshHeader,
													vector<InstanceInstr>* p_outInstanceInstructions)
{
		// set
		ModelResource* model = new ModelResource();
		model->meshHeader = p_meshHeader;
		// Mesh data
		// Raw data extraction
		void* vertices = p_aglMesh->getVertices();
		void* indices = static_cast<void*>(p_aglMesh->getIndices());
		unsigned int numVertices = static_cast<unsigned int>(p_meshHeader->vertexCount);
		unsigned int numIndices =  static_cast<unsigned int>(p_meshHeader->indexCount);
		// Internal mesh format creation
		Mesh* mesh = m_bufferFactory->createMeshFromRaw(vertices, indices,
														numVertices,
														numIndices);
		readAndStoreTextures(p_modelNumber,p_scene,mesh);
		// put in manager			
		unsigned int meshResultId = m_meshManager->addResource(p_name,mesh);	
		// store in model
		model->meshId = static_cast<int>(meshResultId);

		// other model creation data
		readAndStoreEmpties((int)p_modelNumber,model,p_scene,p_outInstanceInstructions);
		readAndStoreParticleSystems(p_modelNumber,model,p_scene);

		// Done
		p_modelCollection->collection.push_back(model); // register in collection
}

void ModelExtendedManagedFactory::readAndStoreTextures( unsigned int p_modelNumber, 
												 AglScene* p_scene,
												 Mesh* p_mesh)
{
	vector<AglMaterialMapping> mmap = p_scene->getMaterialMappings();
	int matId = mmap[p_modelNumber].materialID;
	AglMaterial* mat = p_scene->getMaterial(matId);
	// get names
	string diffuseName = p_scene->getName(mat->diffuseTextureNameIndex);
	string specularName = p_scene->getName(mat->specularTextureNameIndex);
	string normalName = p_scene->getName(mat->normalTextureNameIndex);
	// Create material
	MaterialInfo materialInfo;
	materialInfo.setTextureId(MaterialInfo::DIFFUSEMAP, 
		m_textureFactory->createTexture(diffuseName,TEXTUREPATH));
	materialInfo.setTextureId(MaterialInfo::SPECULARMAP,
		m_textureFactory->createTexture(specularName,TEXTUREPATH));
	materialInfo.setTextureId(MaterialInfo::NORMALMAP,
		m_textureFactory->createTexture(specularName,TEXTUREPATH));
	// and then set the resulting data to the mesh
	p_mesh->setMaterial(materialInfo);
}

void ModelExtendedManagedFactory::readAndStoreEmpties( int p_modelNumber, 
													   ModelResource* p_model, 
													   AglScene* p_scene,
													   vector<InstanceInstr>* p_outInstanceInstructions)
{
	unsigned int connectionPoints = p_scene->getConnectionPointCount();
	for (unsigned int n=0;n<connectionPoints;n++)
	{
		AglConnectionPoint* cp = &p_scene->getConnectionPoint(n);
		// parse name
		string name = p_scene->getName(cp->nameID);
		pair<MeshNameScriptParser::Data,MeshNameScriptParser::Token> parsedAction;
		parsedAction = MeshNameScriptParser::parse(name);
		// Actions based on parsed name
		switch (parsedAction.second) 
		{
		case MeshNameScriptParser::INSTANTIATE: // instantiate
			{
				if (cp->parentMesh == p_modelNumber) // handle global and local call the same
				{
					InstanceInstr inst = {parsedAction.first.filename,
						cp->transform};
					p_outInstanceInstructions->push_back(inst);
				}
				break;
			}

		case MeshNameScriptParser::CONNECTIONPOINT: // normal cp
		default:				
			{
				if (p_modelNumber!=-1) // call from parent
				{
					if (cp->parentMesh == p_modelNumber)
					{
						p_model->connectionPoints->m_collection.push_back(cp->transform);
					}
				}
				else // call from global
				{
					// make pointed model to parent
					if (cp->parentMesh == -1 && p_model!=NULL)
						p_model->connectionPoints->m_collection.push_back(cp->transform);
				}

				break;
			}

		}

	}
}

ModelResource* ModelExtendedManagedFactory::getFallback()
{
	string errname = "__ERROR__MESH__";
	int meshFoundId = m_meshManager->getResourceId(errname);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
		ModelResource* model = new ModelResource();
		model->name = errname;
		// fallback mesh and texture
		Mesh* mesh = m_bufferFactory->createBoxMesh();
		MaterialInfo materialInfo;
		unsigned int meshResultId = m_meshManager->addResource(errname,mesh);
		materialInfo.setTextureId( MaterialInfo::DIFFUSEMAP, 
			m_textureFactory->createTexture("mesherror.png",TEXTUREPATH));
		materialInfo.setTextureId(MaterialInfo::NORMALMAP,
			m_textureFactory->createTexture("mesherror.png",TEXTUREPATH));
		mesh->setMaterial(materialInfo);
		model->meshId = meshResultId;
		// add to manager
		m_modelResourceCache->addResource(errname,new ModelResourceCollection(model));
		return model;
	}
	else
	{
		return m_modelResourceCache->getResource(errname)->collection[0];
	}
}

ModelResource* ModelExtendedManagedFactory::getBox()
{
	string errname = m_primitiveBoxName;
	int meshFoundId = m_meshManager->getResourceId(errname);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
		ModelResource* model = new ModelResource();
		model->name = errname;
		// fallback mesh and texture
		Mesh* mesh = m_bufferFactory->createBoxMesh();
		MaterialInfo materialInfo;
		unsigned int meshResultId = m_meshManager->addResource(errname,mesh);
		materialInfo.setTextureId( MaterialInfo::DIFFUSEMAP, 
			m_textureFactory->createTexture("testtexture.png",TESTTEXTUREPATH));
		materialInfo.setTextureId(MaterialInfo::NORMALMAP,
			m_textureFactory->createTexture("testtexture.png",TESTTEXTUREPATH));
		mesh->setMaterial(materialInfo);
		model->meshId = meshResultId;
		// add to manager
		m_modelResourceCache->addResource(errname,new ModelResourceCollection(model));
		return model;
	}
	else
	{
		return m_modelResourceCache->getResource(errname)->collection[0];
	}
}

ModelResource* ModelExtendedManagedFactory::getSphere()
{
	string errname = m_primitiveSphereName;
	int meshFoundId = m_meshManager->getResourceId(errname);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
		ModelResource* model = new ModelResource();
		model->name = errname;
		// fallback mesh and texture
		Mesh* mesh = m_bufferFactory->createSphereMesh;
		MaterialInfo materialInfo;
		unsigned int meshResultId = m_meshManager->addResource(errname,mesh);
		materialInfo.setTextureId( MaterialInfo::DIFFUSEMAP, 
			m_textureFactory->createTexture("testtexture.png",TESTTEXTUREPATH));
		materialInfo.setTextureId(MaterialInfo::NORMALMAP,
			m_textureFactory->createTexture("testtexture.png",TESTTEXTUREPATH));
		mesh->setMaterial(materialInfo);
		model->meshId = meshResultId;
		// add to manager
		m_modelResourceCache->addResource(errname,new ModelResourceCollection(model));
		return model;
	}
	else
	{
		return m_modelResourceCache->getResource(errname)->collection[0];
	}
}
