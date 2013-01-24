#include "ModelExtendedManagedFactory.h"
#include <AglScene.h>
#include <Globals.h>
#include "MaterialInfo.h"
#include "Mesh.h"
#include "MeshNameScriptParser.h"

const string& ModelExtendedManagedFactory::primitiveCubeName="P_cube";
const string& ModelExtendedManagedFactory::primitiveSphereName="P_sphere";
const string& ModelExtendedManagedFactory::fallbackTextureName="testtexture.png";
const string& ModelExtendedManagedFactory::mesherrorTextureName="mesherror.png";
const string& ModelExtendedManagedFactory::defaultTextureName="defaulttexture.png";

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
	int modelFoundId = m_modelResourceCache->getResourceId(p_name);
	if (modelFoundId==-1)  // if it does not exist, create new
	{
		if (p_name == primitiveCubeName)
		{
			// DEBUGWARNING(( string("Cube").c_str() ));
			model = getCube();
		}
		else if (p_name == primitiveSphereName)
		{
			// DEBUGWARNING(( string("Sphere").c_str() ));
			model = getSphere();
		}
		else
		{
			// Check and read the file
			AglScene* scene = readScene(p_name,p_path);
			//
			if (scene)
			{ 
				InstanceInstruction currentInstance={p_name,AglMatrix::identityMatrix()};

				// DEBUGWARNING(( ("Loading mesh from "+currentInstance.filename+" single instance").c_str() ));

				vector<ModelResource*>* models = createAllModelData(&currentInstance,
												 scene,1,NULL);
				if ((*models)[0]!=NULL)
				{
					model = (*models)[0];
					readAndStoreEmpties(-1,model,model->transform,
						scene,&currentInstance); // read leftover empties
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
											scene->getMeshes().size(),
											instanceInstructions);		
					// read leftover empties
					if ((*models)[0]!=NULL)
					{
						ModelResource* model = (*models)[0];
						readAndStoreEmpties(-1,model,model->transform,
							scene,&currentInstance,instanceInstructions);
					}
				}
				else
				{
					// an instance needs to add(instead of copy) to original collection from 
					// the instance's read collection.
					// it also needs to copy the model resource data
					vector<ModelResource*>* prefetched = createAllModelData(&currentInstance,
																			scene,
																			scene->getMeshes().size(),
																			instanceInstructions);
					// read leftover empties
					if ((*prefetched)[0]!=NULL)
					{
						ModelResource* model = (*prefetched)[0];
						readAndStoreEmpties(-1,model,currentInstance.transform,
							scene,&currentInstance,instanceInstructions); 
					}
					//
					int size = prefetched->size();
					for (int n=firstMeshPos;n<size;n++)
					{
						ModelResource* model = new ModelResource( *(*prefetched)[n] );
						// mesh transform
						model->transform = model->transform*currentInstance.transform;
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
			if (instanceCount==0) 
			{
				// just a normal mesh, just copy resource instructions
				models = &m_modelResourceCache->getResource(currentInstance.filename)->collection;
			}
			else					
			{
				// an instance needs to add to original collection from  the instance's collection
				// it also needs to copy the model resource data
				vector<ModelResource*>* prefetched = &m_modelResourceCache->getResource(currentInstance.filename)->collection;

				int size = prefetched->size();
				for (int n=firstMeshPos;n<size;n++)
				{
					ModelResource* model = new ModelResource( *(*prefetched)[n] );
					// mesh transform
					model->transform *= currentInstance.transform;
					// instances
					for (int n=0;n<model->instances.size();n++)
					{
						InstanceInstruction instruction = model->instances[n];
						instruction.transform *= currentInstance.transform;
						instanceInstructions->push_back(instruction);
					}
					// 
					models->push_back(model);
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


vector<ModelResource*>* ModelExtendedManagedFactory::createAllModelData( const InstanceInstruction* p_instanceData,  
																  AglScene* p_scene, 
																  unsigned int p_numberOfModels,
																  vector<InstanceInstruction>* p_outInstanceInstructions)
{
	p_numberOfModels = max(p_numberOfModels,1);
	ModelResourceCollection* models = new ModelResourceCollection();
	models->collection.push_back(new ModelResource(p_instanceData->filename+"-ROOT"));
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
			// DEBUGWARNING(( ("Creating mesh "+meshName).c_str() ));
			// Actions based on parsed name
			switch (parsedAction.second) 
			{
			case MeshNameScriptParser::INSTANTIATE: // instantiate
				{
					if (p_outInstanceInstructions!=NULL)
					{
						InstanceInstruction inst = {parsedAction.first.filename,
			                                  aglMeshHeader.transform};

						DEBUGWARNING(("Mesh with instancing instruction, empties are recommended instead. Unexpected behaviour may occur."));
						// Not possible yet-> p_model->instances.push_back(inst);
						p_outInstanceInstructions->push_back(inst);
					}

					break;
				}

			case MeshNameScriptParser::MESH: // normal mesh
			default:				
				{
					// DEBUGWARNING(( string("Normal mesh").c_str() ));
					createAndAddModel(models, i, p_instanceData, parsedAction.first.name, 
						p_scene, aglMesh, &aglMeshHeader, p_outInstanceInstructions);
					break;
				}
			}

		}
	}
	m_modelResourceCache->addResource(p_instanceData->filename,models); // register collection in cache

	return &models->collection;
}

void ModelExtendedManagedFactory::createAndAddModel( ModelResourceCollection* p_modelCollection, 
													unsigned int p_modelNumber, 
													const InstanceInstruction* p_instanceData, 
													const string& p_nameSuffix,AglScene* p_scene, AglMesh* p_aglMesh,
													AglMeshHeader* p_meshHeader,
													vector<InstanceInstruction>* p_outInstanceInstructions)
{
		// set
		ModelResource* model = new ModelResource();
		model->meshHeader = *p_meshHeader;
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
		string suffix = "_"+p_nameSuffix;
		if (p_modelNumber==0) suffix="";
		unsigned int meshResultId = m_meshManager->addResource(p_instanceData->filename+suffix,
															   mesh);	
		// store in model
		model->name = p_instanceData->filename+suffix;
		model->meshId = static_cast<int>(meshResultId);
		model->transform = p_meshHeader->transform;

		// other model creation data
		readAndStoreEmpties((int)p_modelNumber,model,model->transform,
			p_scene,p_instanceData,p_outInstanceInstructions);
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
	// diffuse
	string diffuseName = defaultTextureName;
	if (mat->diffuseTextureNameIndex!=-1)
		diffuseName = p_scene->getName(mat->diffuseTextureNameIndex);
	// specular
	string specularName = defaultTextureName;
	if (mat->specularTextureNameIndex!=-1)
		specularName = p_scene->getName(mat->specularTextureNameIndex);
	// normal
	string normalName = defaultTextureName;
	bool hasNormalMap=false;
	if (mat->normalTextureNameIndex!=-1)
	{
		hasNormalMap=true;
		normalName = p_scene->getName(mat->normalTextureNameIndex);
	}
	// displacement
	string dispName = defaultTextureName;
	bool hasDisplacementMap=false;
	if (mat->displacementTextureNameIndex!=-1)
	{
		hasDisplacementMap=true;
		dispName = p_scene->getName(mat->displacementTextureNameIndex);
	}
	// glow
	string glowName = defaultTextureName;
	if (mat->glowTextureNameIndex!=-1)
		glowName = p_scene->getName(mat->glowTextureNameIndex);

	// Create material
	MaterialInfo materialInfo;
	materialInfo.hasNormalMap = hasNormalMap;
	materialInfo.hasDisplacementMap = hasDisplacementMap;
	materialInfo.setTextureId(MaterialInfo::DIFFUSEMAP, 
		m_textureFactory->createTexture(diffuseName,TEXTUREPATH));
	materialInfo.setTextureId(MaterialInfo::SPECULARMAP,
		m_textureFactory->createTexture(specularName,TEXTUREPATH));
	materialInfo.setTextureId(MaterialInfo::NORMALMAP,
		m_textureFactory->createTexture(normalName,TEXTUREPATH));
	materialInfo.setTextureId(MaterialInfo::DISPLACEMENTMAP,
		m_textureFactory->createTexture(dispName,TEXTUREPATH));
	materialInfo.setTextureId(MaterialInfo::GLOWMAP,
		m_textureFactory->createTexture(glowName,TEXTUREPATH));
	// and then set the resulting data to the mesh
	p_mesh->setMaterial(materialInfo);
}

void ModelExtendedManagedFactory::readAndStoreEmpties( int p_modelNumber, 
													   ModelResource* p_model, 
													   AglMatrix& p_offset,
													   AglScene* p_scene,
													   const InstanceInstruction* p_instanceData, 
													   vector<InstanceInstruction>* p_outInstanceInstructions)
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
					InstanceInstruction inst = {parsedAction.first.filename,
						cp->transform*p_offset};
					// DEBUGWARNING(( ("Found instance "+parsedAction.first.filename).c_str() ));
					p_model->instances.push_back(inst);
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
						// DEBUGWARNING(( string("Found connection point for mesh!").c_str() ));
						p_model->connectionPoints.m_collection.push_back(cp->transform*p_offset);
					}
				}
				else // call from global
				{
					// make pointed model to parent
					if (cp->parentMesh == -1 && p_model!=NULL)
					{
						// DEBUGWARNING(( string("Found global connection point!").c_str() ));
						p_model->connectionPoints.m_collection.push_back(cp->transform*p_offset);
					}
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
			m_textureFactory->createTexture(mesherrorTextureName,TEXTUREPATH));
		materialInfo.setTextureId(MaterialInfo::NORMALMAP,
			m_textureFactory->createTexture(mesherrorTextureName,TEXTUREPATH));
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

ModelResource* ModelExtendedManagedFactory::getCube()
{
	string errname = primitiveCubeName;
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
			m_textureFactory->createTexture(fallbackTextureName,TESTTEXTUREPATH));
		materialInfo.setTextureId(MaterialInfo::NORMALMAP,
			m_textureFactory->createTexture(fallbackTextureName,TESTTEXTUREPATH));
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
	string errname = primitiveSphereName;
	int meshFoundId = m_meshManager->getResourceId(errname);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
		ModelResource* model = new ModelResource();
		model->name = errname;
		// fallback mesh and texture
		Mesh* mesh = m_bufferFactory->createSphereMesh();
		MaterialInfo materialInfo;
		unsigned int meshResultId = m_meshManager->addResource(errname,mesh);
		materialInfo.setTextureId( MaterialInfo::DIFFUSEMAP, 
			m_textureFactory->createTexture(fallbackTextureName,TESTTEXTUREPATH));
		materialInfo.setTextureId(MaterialInfo::NORMALMAP,
			m_textureFactory->createTexture(fallbackTextureName,TESTTEXTUREPATH));
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

bool ModelExtendedManagedFactory::isMirrorMatrix(const AglMatrix& p_matrix )
{
	AglVector3 row1 = AglVector3(p_matrix.data[0],p_matrix.data[1],p_matrix.data[2]);
	AglVector3 row2 = AglVector3(p_matrix.data[3],p_matrix.data[4],p_matrix.data[5]);
	AglVector3 row3 = AglVector3(p_matrix.data[6],p_matrix.data[7],p_matrix.data[8]);
	return AglVector3::dotProduct(AglVector3::crossProduct(row1,row2),row3)>0.0f?false:true;
}
