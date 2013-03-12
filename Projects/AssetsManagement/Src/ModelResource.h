#pragma once

#include <AglMesh.h>
#include "ConnectionPointCollection.h"
#include "SpawnPointCollection.h"
#include "ParticleSystemInstructionCollection.h"
#include "LightDataCollection.h"
#include "InstanceInstruction.h"
#include "ParticleSystemInstruction.h"
#include "AmbientRangeSoundDataCollection.h"
#include <AglLooseBspTree.h>
#include <AglScene.h>


// =======================================================================================
//                                    ModelResource
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A model object containing data for describing a collection of mesh related
/// data to be used during loading and before they're put into ES
///        
/// # ModelResource
/// Detailed description.....
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

struct ModelResource
{
public:
    ModelResource() {meshId=-1; name="noname"; 
					 looseBspTree = NULL; transform=AglMatrix::identityMatrix(); scene = NULL;}

    ModelResource(const ModelResource& p_copy)
    {
        meshId = p_copy.meshId;
        name = p_copy.name;
        meshHeader = p_copy.meshHeader;
        connectionPoints = p_copy.connectionPoints;
		spawnPoints = p_copy.spawnPoints;
		lightCollection = p_copy.lightCollection;
        particleSystems = p_copy.particleSystems;
        transform = p_copy.transform;
		instances = p_copy.instances;
		looseBspTree = p_copy.looseBspTree;
		scene = p_copy.scene;
    }	

    ModelResource(const string& p_name) {transform=AglMatrix::identityMatrix(); 
										 meshId=-1; name=p_name; looseBspTree = NULL; scene = NULL;}
    virtual ~ModelResource()
	{
	}

    int							meshId;
    string						name;
    AglMeshHeader				meshHeader;
    ConnectionPointCollection	connectionPoints;
	SpawnPointCollection		spawnPoints;
	LightDataCollection			lightCollection;
	AmbientRangeSoundDataCollection ambientRangeSoundCollection;
    ParticleSystemInstructionCollection	particleSystems;
    AglMatrix					transform;
    vector<InstanceInstruction> instances;
    AglLooseBspTree*			looseBspTree;
	AglScene*					scene;
};