#include "ModelExtendedFactory.h"
#include <Globals.h>
#include <AglScene.h>
#include <AglReader.h>
#include <time.h>
#include "MaterialInfo.h"
#include "Mesh.h"

#define FORCE_VS_DBG_OUTPUT

// Primitives
const string& ModelExtendedFactory::primitiveCubeName="P_cube";
const string& ModelExtendedFactory::primitiveSphereName="P_sphere";
// Fallback- and default textures
const string& ModelExtendedFactory::fallbackTextureName="testtexture.png";
const string& ModelExtendedFactory::mesherrorTextureName="mesherror.png";
const string& ModelExtendedFactory::defaultDiffuseTextureName="defaultdiffuse.png";
const string& ModelExtendedFactory::defaultSpecularTextureName="defaultspecular.png";
const string& ModelExtendedFactory::defaultNormalTextureName="defaultnormal.png";
const string& ModelExtendedFactory::defaultGradientTextureName="defaultgradient.png";

ModelExtendedFactory::ModelExtendedFactory(ID3D11Device* p_device,BufferFactory* p_bufferFactory, 
	ResourceManager<Mesh>* p_resourceManager,
	TextureFactory* p_textureFactory) : ModelBaseFactory()
{
	m_device = p_device;
	m_bufferFactory = p_bufferFactory;
	m_meshManager = p_resourceManager;
	m_textureFactory = p_textureFactory;
}

ModelExtendedFactory::~ModelExtendedFactory()
{
	
}

vector<ModelResource*>* ModelExtendedFactory::createModelResources( const string& p_name,
																   const string* p_path/*=NULL*/, 
																   bool p_isPrimitive )
{
	// performance test by Jarl
	clock_t init, final;
	init=clock();

	vector<ModelResource*>* models=NULL;

	if (p_isPrimitive)
	{
		models=createPrimitive(p_name);
	}
	else
	{
		models=ModelBaseFactory::createModelResources(p_name,p_path);
	}

	final=clock()-init;
	DEBUGPRINT(( ("\n"+p_name+" loaded in "+toString((double)final / ((double)CLOCKS_PER_SEC))+" seconds.\n").c_str() ));

	return models;
}


vector<ModelResource*>* ModelExtendedFactory::createPrimitive( const string& p_name )
{
	vector<ModelResource*>* model = NULL;
	// check if resource already exists
	// unsigned int meshResultId = 0;
	int modelFoundId = m_modelResourceCache->getResourceId(p_name);
	if (modelFoundId==-1)  // if it does not exist, create new
	{
		if (p_name == primitiveCubeName)
		{
			model = getCube();
		}
		else if (p_name == primitiveSphereName)
		{
			model = getSphere();
		}
		else
		{
			model = getCube(); // fallback
		}
	}
	else // the mesh already exists
	{
		model = &m_modelResourceCache->getResource(p_name)->collection;
	}

	return model;
}


unsigned int ModelExtendedFactory::createMeshData( SourceData& p_source,
												  const InstanceInstruction* p_instanceData)
{
	// Raw data extraction
	void* vertices = p_source.aglMesh->getVertices();
	void* indices = static_cast<void*>(p_source.aglMesh->getIndices());
	unsigned int numVertices = static_cast<unsigned int>(p_source.meshHeader->vertexCount);
	unsigned int numIndices =  static_cast<unsigned int>(p_source.meshHeader->indexCount);

	//Also extract any skeleton mappings that can be found
	void* skeletonVertices = NULL;
	unsigned int numSkeletonVertices = 0;

	vector<AglSkeletonMapping*> sm = p_source.scene->getSkeletonMappings();
	for (unsigned int i = 0; i < sm.size(); i++)
	{
		if (sm[i]->getHeader().meshID == p_source.meshHeader->id)
		{
			skeletonVertices = static_cast<void*>(sm[i]->getVertices());
			numSkeletonVertices = static_cast<unsigned int>(sm[i]->getHeader().vertexCount);
			break;
		}
	}


	// Internal mesh format creation
	Mesh* mesh = m_bufferFactory->createMeshFromRaw(vertices, skeletonVertices, indices,
		numVertices,
		numSkeletonVertices,
		numIndices);
	readAndStoreTextures(p_source, mesh);
	// put in manager			
	string meshname = p_instanceData->filename+p_source.nameSuffix;
	unsigned int meshResultId = m_meshManager->addResource(meshname, mesh);
	return meshResultId;
}



void ModelExtendedFactory::readAndStoreTextures( SourceData& p_source,
												 Mesh* p_mesh)
{
	vector<AglMaterialMapping> mmap = p_source.scene->getMaterialMappings();
	int matId = mmap[p_source.modelNumber].materialID;
	AglMaterial* mat = p_source.scene->getMaterial(matId);
	AglGradient* gradients = NULL;
	// get names
	// diffuse
	string diffuseName = defaultDiffuseTextureName;
	if (mat->diffuseTextureNameIndex!=-1)
		diffuseName = p_source.scene->getName(mat->diffuseTextureNameIndex,true);
	// specular
	string specularName = defaultSpecularTextureName;
	if (mat->specularTextureNameIndex!=-1)
		specularName = p_source.scene->getName(mat->specularTextureNameIndex,true);
	// normal
	string normalName = defaultNormalTextureName;
	if (mat->normalTextureNameIndex!=-1)
	{
		normalName = p_source.scene->getName(mat->normalTextureNameIndex,true);
	}
	// displacement
	string dispName = "";
	bool hasDisplacementMap=false;
	if (mat->displacementTextureNameIndex!=-1)
	{
		hasDisplacementMap=true;
		dispName = p_source.scene->getName(mat->displacementTextureNameIndex,true);
	}
	// glow
	string glowName = defaultSpecularTextureName;
	if (mat->glowTextureNameIndex!=-1)
		glowName = p_source.scene->getName(mat->glowTextureNameIndex,true);
	// gradient
	string gradientName = defaultGradientTextureName;
	if (mat->gradientTextureNameIndex!=-1){
		gradients = p_source.scene->getGradient(mat->gradientDataIndex);
		gradientName = p_source.scene->getName(mat->gradientTextureNameIndex,true);

	}

	// Create material
	MaterialInfo materialInfo;
	materialInfo.setTextureId(MaterialInfo::DIFFUSEMAP, 
		m_textureFactory->createTexture(diffuseName,TEXTUREPATH));
	materialInfo.setTextureId(MaterialInfo::SPECULARMAP,
		m_textureFactory->createTexture(specularName,TEXTUREPATH));
	materialInfo.setTextureId(MaterialInfo::NORMALMAP,
		m_textureFactory->createTexture(normalName,TEXTUREPATH));

	materialInfo.hasDisplacementMap = hasDisplacementMap;
	if(hasDisplacementMap){
		materialInfo.setTextureId(MaterialInfo::DISPLACEMENTMAP,
			m_textureFactory->createTexture(dispName,TEXTUREPATH));
	}

	materialInfo.setTextureId(MaterialInfo::GLOWMAP,
		m_textureFactory->createTexture(glowName,TEXTUREPATH));
	materialInfo.setTextureId(MaterialInfo::GRADIENTMAP,
		m_textureFactory->createTexture(gradientName, TEXTUREPATH));

	if(gradients != NULL){
		for (unsigned int i = 0 ; i < gradients->getLayers().size(); i++){
			materialInfo.setGradientLayer(i,gradients->getLayerColor(i));
		}
		materialInfo.setNumberOfGradientLayers(gradients->getLayers().size());
	}

	// and then set the resulting data to the mesh, 
	p_mesh->setMaterial(materialInfo);
}


ModelResource* ModelExtendedFactory::getFallback()
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

vector<ModelResource*>* ModelExtendedFactory::getCube()
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
		ModelResourceCollection* res = new ModelResourceCollection(model);
		m_modelResourceCache->addResource(errname,res);
		return &res->collection;
	}
	else
	{
		return &m_modelResourceCache->getResource(errname)->collection;
	}
}

vector<ModelResource*>* ModelExtendedFactory::getSphere()
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
		ModelResourceCollection* res = new ModelResourceCollection(model);
		m_modelResourceCache->addResource(errname,res);
		return &res->collection;
	}
	else
	{
		return &m_modelResourceCache->getResource(errname)->collection;
	}
}

// bool ModelExtendedFactory::isMirrorMatrix(const AglMatrix& p_matrix )
// {
// 	AglVector3 row1 = AglVector3(p_matrix.data[0],p_matrix.data[1],p_matrix.data[2]);
// 	AglVector3 row2 = AglVector3(p_matrix.data[3],p_matrix.data[4],p_matrix.data[5]);
// 	AglVector3 row3 = AglVector3(p_matrix.data[6],p_matrix.data[7],p_matrix.data[8]);
// 	return AglVector3::dotProduct(AglVector3::crossProduct(row1,row2),row3)>0.0f?false:true;
// }
