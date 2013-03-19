#include "ModelResource.h"


ModelResource::ModelResource()
{

}

ModelResource::ModelResource( const ModelResource& p_copy )
{
	meshId = p_copy.meshId;
	name = p_copy.name;
	meshHeader = p_copy.meshHeader;
	connectionPoints = p_copy.connectionPoints;
	spawnPoints = p_copy.spawnPoints;
	lightCollection = p_copy.lightCollection;
	int s = p_copy.ambientRangeSoundCollection.m_collection.size();
	ambientRangeSoundCollection = p_copy.ambientRangeSoundCollection;
	particleSystems = p_copy.particleSystems;
	transform = p_copy.transform;
	instances = p_copy.instances;
	looseBspTree = p_copy.looseBspTree;
	scene = p_copy.scene;	
}

ModelResource::ModelResource( const string& p_name )
{
	transform=AglMatrix::identityMatrix(); 
	meshId=-1; name=p_name; looseBspTree = NULL; scene = NULL;
}
