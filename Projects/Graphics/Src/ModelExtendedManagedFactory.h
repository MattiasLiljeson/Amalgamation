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


	virtual ModelResource* createModelResource(const string& p_name,
		const string* p_path=NULL);

	virtual vector<ModelResource*>* createModelResources(const string& p_name,
		const string* p_path=NULL);

private:
	struct ModelResourceCollection
	{
		ModelResourceCollection() {}
		ModelResourceCollection(ModelResource* p_val)
		{
			collection.push_back(p_val);
		}

		~ModelResourceCollection()
		{
			for (int i=0;i<collection.size();i++)
			{
				delete collection[i];
			}
		}
		vector<ModelResource*> collection;
	};


	virtual vector<ModelResource*>* createAllModelData(const string& p_name, AglScene* p_scene, 
		unsigned int p_numberOfModels);

	void readAndStoreTextures(unsigned int p_modelNumber, AglScene* p_scene, 
							  Mesh* p_mesh);


	virtual ModelResource* getFallback();


	ModelResource* getBox();

	ModelResource* getSphere();

	BufferFactory* m_bufferFactory;
	ResourceManager<Mesh>* m_meshManager;
	TextureFactory* m_textureFactory;
	ID3D11Device* m_device;

	static const string& m_primitiveBoxName;
	static const string& m_primitiveSphereName;

	/// 
	/// Used for when a mesh has already been loaded and the creation instructions 
	/// are needed again without loading the file again.
	ResourceManager<ModelResourceCollection>* m_modelResourceCache; 
};