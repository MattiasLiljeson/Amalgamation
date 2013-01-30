#pragma once
#include <string>
#include <vector>
#include <FileCheck.h>
#include "InstanceInstruction.h"
#include "ModelResource.h"
#include "MeshLoadException.h"
#include "ResourceManager.h"

class AglScene;

using namespace std;

// =======================================================================================
//                                   ModelBaseFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	The ModelBaseUnmanagedFactory can read agl files but won't do anything 
//  DX-related.
///        
/// # ModelBaseFactory
/// Detailed description.....
/// Created on: 15-1-2013 
/// Since 30-1-2013 this base class is also managed(can handle storage).
///---------------------------------------------------------------------------------------

class ModelBaseFactory
{
public:
	ModelBaseFactory();
	virtual ~ModelBaseFactory();

	///-----------------------------------------------------------------------------------
	/// Returns a pointer to a stored ModelResource, thusly the pointer should 
	/// not be deleted.
	/// \param p_name
	/// \param p_path
	/// \return ModelResource*
	/// Deprecated since 30-1-2013
	///-----------------------------------------------------------------------------------
	virtual ModelResource* createModelResource_DEPRECATED(const string& p_name,
										  const string* p_path=NULL);

	///-----------------------------------------------------------------------------------
	/// Creates a vector with model resource instructions. All managed, no deletion outside.
	/// \param p_name
	/// \param p_path
	/// \return vector<ModelResource*>*
	///-----------------------------------------------------------------------------------
	virtual vector<ModelResource*>* createModelResources(const string& p_name,
												    const string* p_path=NULL);


protected:
	///-----------------------------------------------------------------------------------
	/// ModelResourceCollection contains several ModelResource references. It is used
	/// for internal storage and handling of the data returned from the creation methods.
	///-----------------------------------------------------------------------------------
	struct ModelResourceCollection
	{
		ModelResourceCollection() {rootIndex=-1;collection = vector<ModelResource*>();}
		ModelResourceCollection(ModelResource* p_val)
		{
			rootIndex=-1;
			collection.push_back(p_val);
		}

		~ModelResourceCollection()
		{
			for (unsigned int i=0;i<collection.size();i++)
			{
				delete collection[i];
			}
		}
		int rootIndex;
		vector<ModelResource*> collection;
	};
	///-----------------------------------------------------------------------------------
	/// SourceData used during creation to decrease the amount of parameters needed to be
	/// passed around.
	///----------------------------------------------------------------------------------
	struct SourceData
	{
		AglScene* scene;
		AglMesh* aglMesh;
		AglMeshHeader* meshHeader;
		unsigned int modelNumber;
		string& nameSuffix;
	};
protected:

	///-----------------------------------------------------------------------------------
	/// Read Agl-scene from file
	/// \param p_name
	/// \param p_path
	/// \return AglScene*
	///-----------------------------------------------------------------------------------
	AglScene* readScene(const string& p_name, const string* p_path);

	///-----------------------------------------------------------------------------------
	/// Construct all model data from an Agl-scene
	/// \param p_instanceData Information struct on what model to create.
	/// \param p_scene The current scene to read from.
	/// \param p_numberOfModels The maximum number of models to read.
	/// \param p_outInstanceInstructions Outputs further creation instructions.
	/// \return vector<ModelResource*>*
	///-----------------------------------------------------------------------------------
	virtual vector<ModelResource*>* createAllModelData(const InstanceInstruction* p_instanceData, 
		AglScene* p_scene, 
		vector<InstanceInstruction>* p_outInstanceInstructions=NULL);

	///-----------------------------------------------------------------------------------
	/// Creates a single model and adds it to the model collection.
	/// \param p_modelCollection
	/// \param p_instanceData
	/// \param p_source
	/// \param p_outInstanceInstructions
	/// \return void
	///-----------------------------------------------------------------------------------
	virtual void createAndAddModel(ModelResourceCollection* p_modelCollection, 
		const InstanceInstruction* p_instanceData, 
		SourceData& p_source,
		vector<InstanceInstruction>* p_outInstanceInstructions=NULL);

	///-----------------------------------------------------------------------------------
	/// Handles a single mesh. The base class will only return -1.
	/// \param p_source
	/// \return unsigned int
	///-----------------------------------------------------------------------------------
	virtual unsigned int createMeshData(SourceData& p_source,
		const InstanceInstruction* p_instanceData);

	///-----------------------------------------------------------------------------------
	/// Reads and stores empties of a certain model.
	/// \param p_modelNumber The number of the model of which to read the empties. Use -1
	/// for reading empties in root.
	/// \param p_model Pointer to modelresource to store empties in.
	/// \param p_offset Transformation of parent model.
	/// \param p_scene The current scene to read from.
	/// \param p_instanceData Information struct on what model to create.
	/// \param p_outInstanceInstructions Outputs further creation instructions.
	/// \return void
	///-----------------------------------------------------------------------------------
	virtual void readAndStoreEmpties(SourceData& p_source,
		ModelResource* p_model,
		AglMatrix& p_offset, 
		const InstanceInstruction* p_instanceData, 
		vector<InstanceInstruction>* p_outInstanceInstructions=NULL);

	///-----------------------------------------------------------------------------------
	/// Reads and stores all the particle systems of a certain model.
	/// \param p_modelNumber The number of the model of which to read the 
	/// particle systems. Use -1 for reading empties in root.
	/// \param p_model Pointer to modelresource to store empties in.
	/// \param p_scene The current scene to read from.
	/// \return void
	///-----------------------------------------------------------------------------------
	virtual void readAndStoreParticleSystems(SourceData& p_source, 
		ModelResource* p_model);

	///-----------------------------------------------------------------------------------
	/// Returns a pre-defined ModelResource that can be used in case of failure.
	/// \return ModelResource*
	///-----------------------------------------------------------------------------------
	virtual ModelResource* getFallback();

	///-----------------------------------------------------------------------------------
	/// Creates a BSP-tree from mesh data.
	/// \param p_mesh
	/// \return AglLooseBspTree*
	///-----------------------------------------------------------------------------------
	virtual AglLooseBspTree* createBspTree(AglMesh* p_mesh);

protected:
	///-----------------------------------------------------------------------------------
	/// The ModelResourceCache is used for when a mesh has already been loaded 
	/// and the creation instructions are needed again without loading the file again.
	///-----------------------------------------------------------------------------------
	ResourceManager<ModelResourceCollection>* m_modelResourceCache; 
private:
};