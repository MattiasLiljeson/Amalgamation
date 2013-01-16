#pragma once

#include <ModelBaseFactory.h>
#include "TextureFactory.h"
#include "BufferFactory.h"
#include <d3d11.h>

// =======================================================================================
//                                   ModelExtendedFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Extension of the modelresource factory for handling buffer creation as well
///        
/// # ModelExtendedFactory
/// Detailed description.....
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

class ModelExtendedFactory : public ModelBaseFactory
{
public:
	ModelExtendedFactory(ID3D11Device* p_device,BufferFactory* p_bufferFactory, 
						 ResourceManager<Mesh>* p_meshManager,
						 TextureFactory* p_textureFactory);
	virtual ~ModelExtendedFactory();


	virtual ModelResource* createModelResource(const string& p_name,
		const string* p_path=NULL);

	virtual vector<ModelResource*>* createModelResources(const string& p_name,
		const string* p_path=NULL);

protected:

	virtual vector<ModelResource*>* createAllModelData(AglScene* p_scene, 
		unsigned int p_numberOfModels);

	BufferFactory* m_bufferFactory;
	ResourceManager<Mesh>* m_meshManager;
	TextureFactory* m_textureFactory;
	ID3D11Device* m_device;
};