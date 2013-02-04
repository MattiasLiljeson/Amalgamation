#pragma once

#include <AglMesh.h>
#include "ConnectionPointCollection.h"
#include "SpawnPointCollection.h"
#include "ParticleSystemCollection.h"
#include "InstanceInstruction.h"
#include <AglLooseBspTree.h>

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
					 looseBspTree = NULL; transform=AglMatrix::identityMatrix();}

    ModelResource(const ModelResource& p_copy)
    {
        meshId = p_copy.meshId;
        name = p_copy.name;
        meshHeader = p_copy.meshHeader;
        connectionPoints = p_copy.connectionPoints;
        particleSystems = p_copy.particleSystems;
        transform = p_copy.transform;
		instances = p_copy.instances;
        looseBspTree = p_copy.looseBspTree;
    }	

    ModelResource(const string& p_name) {transform=AglMatrix::identityMatrix(); 
										 meshId=-1; name=p_name; looseBspTree = NULL;}
    virtual ~ModelResource(){}

    int							meshId;
    string						name;
    AglMeshHeader				meshHeader;
    ConnectionPointCollection	connectionPoints;
	SpawnPointCollection		spawnPoints;
    ParticleSystemCollection	particleSystems;
    AglMatrix					transform;
    vector<InstanceInstruction> instances;
    AglLooseBspTree*			looseBspTree;
};