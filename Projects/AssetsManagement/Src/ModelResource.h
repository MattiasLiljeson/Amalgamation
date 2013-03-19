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
    ModelResource();

    ModelResource(const ModelResource& p_copy);

    ModelResource(const string& p_name);

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