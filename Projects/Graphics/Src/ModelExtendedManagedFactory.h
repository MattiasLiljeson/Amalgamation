#pragma once


#include <ModelBaseUnmanagedFactory.h>
#include "TextureFactory.h"
#include "BufferFactory.h"
#include <d3d11.h>
#include <ResourceManager.h>

class Mesh;

// =======================================================================================
//                                   ModelExtendedManagedFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Extension of the modelresource factory for handling buffer creation as well
/// it also has handles deletion of models
///        
/// # ModelExtendedManagedFactory
/// Detailed description.....
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

class ModelExtendedManagedFactory : public ModelBaseUnmanagedFactory
{
public:
	ModelExtendedManagedFactory(ID3D11Device* p_device,BufferFactory* p_bufferFactory, 
						 ResourceManager<Mesh>* p_meshManager,
						 TextureFactory* p_textureFactory);
	virtual ~ModelExtendedManagedFactory();


	///-----------------------------------------------------------------------------------
	/// Returns a pointer to a stored ModelResource, thusly the pointer should 
	/// not be deleted.
	/// \param p_name
	/// \param p_path
	/// \return ModelResource*
	///-----------------------------------------------------------------------------------
	virtual ModelResource* createModelResource(const string& p_name,
		const string* p_path=NULL);

	///-----------------------------------------------------------------------------------
	/// Creates a vector with model resource instructions. All managed, no deletion outside.
	/// \param p_name
	/// \param p_path
	/// \return vector<ModelResource*>*
	///-----------------------------------------------------------------------------------
	virtual vector<ModelResource*>* createModelResources(const string& p_name,
		const string* p_path=NULL);

private:
	struct ModelResourceCollection
	{
		ModelResourceCollection() {collection = vector<ModelResource*>();}
		ModelResourceCollection(ModelResource* p_val)
		{
			collection.push_back(p_val);
		}

		~ModelResourceCollection()
		{
			for (unsigned int i=0;i<collection.size();i++)
			{
				delete collection[i];
			}
		}

		vector<ModelResource*> collection;
	};

	struct InstanceInstr
	{
		string filename;
		AglMatrix transform;
		bool uneven; // whether this is an uneven instance in creation hierarchy 
	};


	virtual vector<ModelResource*>* createAllModelData(const InstanceInstr* p_instanceData, AglScene* p_scene, 
		unsigned int p_numberOfModels, vector<InstanceInstr>* p_outInstanceInstructions=NULL);

	void createAndAddModel(ModelResourceCollection* p_modelCollection, 
							unsigned int p_modelNumber, const InstanceInstr* p_instanceData, 
							const string& p_nameSuffix,
							AglScene* p_scene, AglMesh* p_aglMesh,
							AglMeshHeader* p_meshHeader,
							vector<InstanceInstr>* p_outInstanceInstructions=NULL);


	void readAndStoreTextures(unsigned int p_modelNumber, AglScene* p_scene, 
							  Mesh* p_mesh);

	virtual void readAndStoreEmpties(int p_modelNumber, 
		ModelResource* p_model, AglScene* p_scene,
		const ModelExtendedManagedFactory::InstanceInstr* p_instanceData, 
		vector<InstanceInstr>* p_outInstanceInstructions=NULL);


	virtual ModelResource* getFallback();

	bool isMirrorMatrix(const AglMatrix& p_matrix);


	ModelResource* getCube();

	ModelResource* getSphere();

	BufferFactory* m_bufferFactory;
	ResourceManager<Mesh>* m_meshManager;
	TextureFactory* m_textureFactory;
	ID3D11Device* m_device;

	static const string& primitiveCubeName;
	static const string& primitiveSphereName;
	static const string& fallbackTextureName;
	static const string& mesherrorTextureName;
	static const string& defaultTextureName;

	static const unsigned int firstMeshPos=1; // always root on zero

	/// 
	/// Used for when a mesh has already been loaded and the creation instructions 
	/// are needed again without loading the file again.
	ResourceManager<ModelResourceCollection>* m_modelResourceCache; 
};