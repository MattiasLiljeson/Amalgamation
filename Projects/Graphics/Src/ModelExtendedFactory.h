#pragma once


#include <ModelBaseFactory.h>
#include "TextureFactory.h"
#include "BufferFactory.h"
#include <d3d11.h>
#include <ResourceManager.h>
#include <InstanceInstruction.h>

class Mesh;

// =======================================================================================
//                                   ModelExtendedFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Extension of the modelresource factory for handling buffer- 
/// and texture creation as well.
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


	virtual vector<ModelResource*>* createModelResources(const string& p_name,
		const string* p_path,
		bool p_isPrimitive);

protected:

	///-----------------------------------------------------------------------------------
	/// Create a primitive
	/// \param p_name
	/// \return ModelResource*
	///-----------------------------------------------------------------------------------
	vector<ModelResource*>* ModelExtendedFactory::createPrimitive( const string& p_name);

	///-----------------------------------------------------------------------------------
	/// Handles a single mesh. The extended class will generate buffers and store.
	/// \param p_source
	/// \return unsigned int
	///-----------------------------------------------------------------------------------
	virtual unsigned int createMeshData(SourceData& p_source,
		const InstanceInstruction* p_instanceData);

	///-----------------------------------------------------------------------------------
	/// Reads and stores textures for a mesh from an agl-file.
	/// \param p_modelNumber
	/// \param p_scene
	/// \param p_mesh
	/// \return void
	///-----------------------------------------------------------------------------------
	void readAndStoreTextures(SourceData& p_source, Mesh* p_mesh);

	///-----------------------------------------------------------------------------------
	/// Returns a pre-defined ModelResource that can be used in case of failure.
	/// \return ModelResource*
	///-----------------------------------------------------------------------------------
	virtual ModelResource* getFallback();

	///-----------------------------------------------------------------------------------
	/// Generates a pre-defined ModelResource containing a cube.
	/// \return ModelResource*
	///-----------------------------------------------------------------------------------
	vector<ModelResource*>* getCube();

	///-----------------------------------------------------------------------------------
	/// Generates a pre-defined ModelResource containing a sphere.
	/// \return ModelResource*
	///-----------------------------------------------------------------------------------
	vector<ModelResource*>* getSphere();
private:
	BufferFactory* m_bufferFactory;			///< Reference to an outside buffer factory
	ResourceManager<Mesh>* m_meshManager;	///< Reference to an outside mesh manager
	TextureFactory* m_textureFactory;		///< Reference to an outside texture factory
	ID3D11Device* m_device;					
private:
	static const string& primitiveCubeName;
	static const string& primitiveSphereName;
	static const string& fallbackTextureName;
	static const string& mesherrorTextureName;
	static const string& defaultDiffuseTextureName;
	static const string& defaultSpecularTextureName;
	static const string& defaultNormalTextureName;
	static const string& defaultGradientTextureName;
private:
	// bool isMirrorMatrix(const AglMatrix& p_matrix);
};