#include "ModelExtendedFactory.h"

ModelExtendedFactory::ModelExtendedFactory(ID3D11Device* p_device,BufferFactory* p_bufferFactory, 
	ResourceManager<Mesh>* p_resourceManager,
	TextureFactory* p_textureFactory)
{
	m_device = p_device;
	m_bufferFactory = p_bufferFactory;
	m_meshManager = p_resourceManager;
	m_textureFactory = p_textureFactory;
}

ModelResource* ModelExtendedFactory::createModelResource( const string& p_name, 
													 const string* p_path/*=NULL*/)
{
	ModelResource* model = NULL;
	// check if resource already exists
	unsigned int meshResultId = 0;
	int meshFoundId = m_meshManager->getResourceId(p_name);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
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
	}
	else // the mesh already exists
	{
		meshResultId = static_cast<unsigned int>(meshFoundId);

	}

	return model;
}

vector<ModelResource*>* ModelExtendedFactory::createModelResources( const string& p_name, 
															   const string* p_path/*=NULL*/)
{
	vector<ModelResource*>* models = NULL;
	// Check and read the file
	unsigned int meshResultId = 0;
	int meshFoundId = m_meshManager->getResourceId(p_name);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
		AglScene* scene = readScene(p_name,p_path);
		//
		if (scene)
		{ 
			models = createAllModelData(scene,scene->getMeshes().size());
		}
		// cleanup
		delete scene;
	}
	else // the mesh already exists
	{
		meshResultId = static_cast<unsigned int>(meshFoundId);
	}

	return models;
}


vector<ModelResource*>* ModelExtendedFactory::createAllModelData( AglScene* p_scene, 
																 unsigned int p_numberOfModels )
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

			readConnectionPoints(i,model,p_scene);
			readParticleSystems(i,model,p_scene);

			// 

			// Done
			models->push_back(model);
		}
	}

	return models;
}


unsigned int GraphicsWrapper::createMesh( const string& p_name,
										 const string* p_path/*=NULL*/,
										 ConnectionPointCollection* p_outConnectionPoints/*=NULL*/)
{
	// =============================================
	//
	// WORK IN PROGRESS.
	// Will need refactoring.
	//
	// =============================================
	// check if resource already exists
	unsigned int meshResultId = 0;
	int meshFoundId = m_meshManager->getResourceId(p_name);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
		// =============================================
		// PRIMITIVES
		// =============================================
		if (p_name=="P_cube")
		{
			MaterialInfo materialInfo;
			Mesh* mesh = m_bufferFactory->createBoxMesh(); // construct a mesh
			meshResultId = m_meshManager->addResource(p_name,mesh);	   // put in manager
			// (Here you might want to do similar checks for textures/materials
			// For now we have a hard coded texture path, but later on
			// we probably get this path from a mesh file loader or similar.
			materialInfo.setTextureId( MaterialInfo::DIFFUSEMAP, 
				createTexture("10x10.png",TESTTEXTUREPATH));
			materialInfo.setTextureId(MaterialInfo::NORMALMAP,
				createTexture("testtexture.png",TESTTEXTUREPATH));
			// and their managers.)
			// ...
			// and then set the resulting data to the mesh
			mesh->setMaterial(materialInfo);
		}
		else if (p_name=="P_sphere")
		{
			MaterialInfo materialInfo;
			Mesh* mesh = m_bufferFactory->createSphereMesh(); // construct a mesh
			meshResultId = m_meshManager->addResource(p_name,mesh);	   // put in manager
			// (Here you might want to do similar checks for textures/materials
			// For now we have a hard coded texture path, but later on
			// we probably get this path from a mesh file loader or similar.
			materialInfo.setTextureId( MaterialInfo::DIFFUSEMAP, 
				createTexture("10x10.png",TESTTEXTUREPATH));
			materialInfo.setTextureId(MaterialInfo::NORMALMAP,
				createTexture("testtexture.png",TESTTEXTUREPATH));
			// and their managers.)
			// ...
			// and then set the resulting data to the mesh
			mesh->setMaterial(materialInfo);
		}
		else
			// =============================================
			// MODEL FILES
			// =============================================
		{
			// Construct path for loading
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
			//
			if (aglScene)
			{ 
				// ------------------
				// Mesh
				// ------------------
				// only handle one mesh for now.
				AglMesh* aglMesh = aglScene->getMeshes()[0];
				AglMeshHeader aglMeshHeader = aglMesh->getHeader();
				MaterialInfo materialInfo;
				// Raw data extraction
				void* vertices = aglMesh->getVertices();
				void* indices = static_cast<void*>(aglMesh->getIndices());
				unsigned int numVertices = static_cast<unsigned int>(aglMeshHeader.vertexCount);
				unsigned int numIndices =  static_cast<unsigned int>(aglMeshHeader.indexCount);
				// Internal mesh format creation
				Mesh* mesh = m_bufferFactory->createMeshFromRaw(vertices, indices,
					numVertices,
					numIndices);
				// put in manager
				meshResultId = m_meshManager->addResource(p_name,mesh);	
				// (Here you might want to do similar checks for textures/materials
				// For now we have a hard coded texture path, but later on
				// we probably get this path from a mesh file loader or similar.
				materialInfo.setTextureId(MaterialInfo::DIFFUSEMAP, 
					createTexture("testtexture.png",TESTTEXTUREPATH));
				materialInfo.setTextureId(MaterialInfo::NORMALMAP,
					createTexture("testtexture.png",TESTTEXTUREPATH));
				// and their managers.)
				// ...
				// and then set the resulting data to the mesh
				mesh->setMaterial(materialInfo);

				// ------------------
				// Connection points
				// ------------------
				if (p_outConnectionPoints!=NULL)
				{
					for (unsigned int i=0;i<aglScene->getConnectionPointCount();i++)
					{
						RawTransformData dat;
						AglMatrix mat = aglScene->getConnectionPoint(i).transform;
						for (unsigned int n=0;n<16;n++)
							dat.transform[n] = mat.data[n];
						p_outConnectionPoints->m_collection.push_back(dat);
					}
				}
			}
			else
			{
				// fallback mesh and texture
				Mesh* mesh = m_bufferFactory->createBoxMesh();
				MaterialInfo materialInfo;
				meshResultId = m_meshManager->addResource(p_name,mesh);
				materialInfo.setTextureId( MaterialInfo::DIFFUSEMAP, 
					createTexture("mesherror.png",TEXTUREPATH));
				materialInfo.setTextureId(MaterialInfo::NORMALMAP,
					createTexture("testtexture.png",TESTTEXTUREPATH));
				mesh->setMaterial(materialInfo);
			}
			// cleanup
			delete aglScene;
		}
	}
	else // the mesh already exists
	{
		meshResultId = static_cast<unsigned int>(meshFoundId);
		Mesh* mesh = m_meshManager->getResource(meshResultId); // get mesh from id
	}
	return meshResultId;
}
