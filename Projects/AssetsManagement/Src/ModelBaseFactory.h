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

	virtual ModelResource* createModelResource(const string& p_name,
										  const string* p_path=NULL);

	virtual vector<ModelResource*>* createModelResources(const string& p_name,
												    const string* p_path=NULL);


	/*
	// WIP, should not use texture pointer, but texture id
	unsigned int createMesh(const string& p_name,
		int p_numVertices, PNTTBVertex* p_vertices, 
		int p_numIndices, DIndex* p_indices,
		Texture* p_texture=NULL);

	// This is the preferred method for creating meshes from raw data
	unsigned int createMesh(const string& p_name,
		int p_numVertices, PNTTBVertex* p_vertices, 
		int p_numIndices, DIndex* p_indices,
		int p_textureId);
	*/

protected:
	AglScene* readScene(const string& p_name, const string* p_path);

	virtual vector<ModelResource*>* createAllModelData(AglScene* p_scene, 
											   unsigned int p_numberOfModels);

	void readConnectionPoints(unsigned int p_modelNumber, ModelResource* p_model, AglScene* p_scene);

	void readParticleSystems(unsigned int p_modelNumber, ModelResource* p_model, AglScene* p_scene);

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