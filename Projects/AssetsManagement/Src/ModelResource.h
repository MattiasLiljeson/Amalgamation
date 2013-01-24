#pragma once

#include <AglMesh.h>
#include "ConnectionPointCollection.h"
#include "ParticleSystemCollection.h"
#include <AglLooseBspTree.h>


// =======================================================================================
//                                        Model
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A model object containing data for describing a collection of mesh related
/// data to be used during loading and before they're put into ES
///        
/// # Model
/// Detailed description.....
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

struct ModelResource
{
public:
	ModelResource() {meshId=-1; name="noname"; looseBspTree = NULL; transform=AglMatrix::identityMatrix();}
	ModelResource(const ModelResource& p_copy)
	{
		meshId = p_copy.meshId;
		name = p_copy.name;
		meshHeader = p_copy.meshHeader;
		connectionPoints = p_copy.connectionPoints;
		particleSystems = p_copy.particleSystems;
		transform = p_copy.transform;
	}	
	ModelResource(const string& p_name) {transform=AglMatrix::identityMatrix(); meshId=-1; name=p_name;}
	virtual ~ModelResource(){}

	int							meshId;
	string						name;
	AglMeshHeader				meshHeader;
	ConnectionPointCollection	connectionPoints;
	ParticleSystemCollection	particleSystems;
	AglMatrix					transform;
	AglLooseBspTree*			looseBspTree;
};