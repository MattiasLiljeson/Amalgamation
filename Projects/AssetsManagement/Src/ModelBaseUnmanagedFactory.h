#pragma once
#include <string>
#include <vector>
#include "ModelResource.h"
#include <FileCheck.h>
#include "MeshLoadException.h"

class Mesh;
class AglScene;

using namespace std;

// =======================================================================================
//                                   ModelBaseUnmanagedFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	The ModelBaseUnmanagedFactory can read agl files but won't do anything 
//  DX-related, and can't handle storaging.
///        
/// # ModelBaseUnmanagedFactory
/// Detailed description.....
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

class ModelBaseUnmanagedFactory
{
public:
	ModelBaseUnmanagedFactory();
	virtual ~ModelBaseUnmanagedFactory();

	virtual ModelResource* createModelResource(const string& p_name,
										  const string* p_path=NULL);

	virtual vector<ModelResource*>* createModelResources(const string& p_name,
												    const string* p_path=NULL);


protected:
	AglScene* readScene(const string& p_name, const string* p_path);

	virtual vector<ModelResource*>* createAllModelData(AglScene* p_scene, 
											   unsigned int p_numberOfModels);

	void readAndStoreConnectionPoints(unsigned int p_modelNumber, ModelResource* p_model, AglScene* p_scene);

	void readAndStoreParticleSystems(unsigned int p_modelNumber, ModelResource* p_model, AglScene* p_scene);

	virtual ModelResource* getFallback();


	///-----------------------------------------------------------------------------------
	/// Create a mesh using name (and loads if path is specified). Returns a mesh id.
	/// \param p_name Filename
	/// \param p_path Path, without filename
	/// \param p_outHardPoints Optional container for storing connection points.
	/// \return unsigned int Mesh id
	///-----------------------------------------------------------------------------------
// 	unsigned int createMesh(const string& p_name,
// 		const string* p_path=NULL,
// 		ConnectionPointCollection* p_outConnectionPoints=NULL);private:
};