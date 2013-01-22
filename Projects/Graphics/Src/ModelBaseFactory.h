#pragma once
#include <string>
#include <vector>
#include "Model.h"
#include <FileCheck.h>
#include "MeshLoadException.h"

class Mesh;
class AglScene;

using namespace std;

// =======================================================================================
//                                   ModelBaseFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	The ModelBaseFactory can read agl files but won't do anything DX-related, and
/// can't handle storaging.
///        
/// # ModelBaseFactory
/// Detailed description.....
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

class ModelBaseFactory
{
public:
	ModelBaseFactory();
	virtual ~ModelBaseFactory();

	virtual ModelResource* createModelDescription(const string& p_name,
										  const string* p_path=NULL);

	virtual vector<ModelResource*>* createModelDescriptions(const string& p_name,
												    const string* p_path=NULL);

protected:
	AglScene* readScene(const string& p_name, const string* p_path);

	virtual vector<ModelResource*>* createAllModelData(AglScene* p_scene, 
											   unsigned int p_numberOfModels);

	void readConnectionPoints(unsigned int p_modelNumber, ModelResource* p_model, AglScene* p_scene);

	void readParticleSystems(unsigned int p_modelNumber, ModelResource* p_model, AglScene* p_scene);

private:
};