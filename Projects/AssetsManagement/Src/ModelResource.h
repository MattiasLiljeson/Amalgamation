#pragma once

#include <AglMesh.h>
#include "ConnectionPointCollection.h"
#include "ParticleSystemCollection.h"


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
	ModelResource() {meshId=-1; name="noname";}
	virtual ~ModelResource(){}

	int							meshId;
	string						name;
	AglMeshHeader				meshHeader;
	ConnectionPointCollection	connectionPoints;
	ParticleSystemCollection	particleSystems;
	AglMatrix					transform;
};