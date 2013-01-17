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

	///-----------------------------------------------------------------------------------
	/// Returns a pointer to a created ModelResource. It must be deleted manually outside.
	/// \param p_name
	/// \param p_path
	/// \return ModelResource*
	///-----------------------------------------------------------------------------------
	virtual ModelResource* createModelResource(const string& p_name,
										  const string* p_path=NULL);

	///-----------------------------------------------------------------------------------
	/// Returns a pointer to a vector of allocated ModelResources. Vector and all 
	/// ModelResources must be deleted manually.
	/// \param p_name
	/// \param p_path
	/// \return vector<ModelResource*>*
	///-----------------------------------------------------------------------------------
	virtual vector<ModelResource*>* createModelResources(const string& p_name,
												    const string* p_path=NULL);


protected:
	AglScene* readScene(const string& p_name, const string* p_path);

	virtual vector<ModelResource*>* createAllModelData(AglScene* p_scene, 
											   unsigned int p_numberOfModels);

	virtual void readAndStoreEmpties(int p_modelNumber, 
											ModelResource* p_model, AglScene* p_scene);

	void readAndStoreParticleSystems(unsigned int p_modelNumber, ModelResource* p_model, AglScene* p_scene);

	virtual ModelResource* getFallback();


private:
};