#include "LoadMeshSystemClient.h"

#include "BodyInitData.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "ConnectionPointSet.h"
#include "EntityParent.h"
#include "GraphicsBackendSystem.h"
#include "LoadMesh.h"
#include "MeshOffsetTransform.h"
#include "ParticleSystemsComponent.h"
#include "PhysicsBody.h"
#include "RenderInfo.h"
#include "SkeletalAnimation.h"
#include "LightsComponent.h"
#include "Transform.h"
#include <AglSkeletonMapping.h>
#include <GraphicsWrapper.h>
#include <ModelResource.h>
#include "SoundComponent.h"

LoadMeshSystemClient::LoadMeshSystemClient( GraphicsBackendSystem* p_gfxBackend ) : 
	LoadMeshSystem()
{
	m_gfxBackend = p_gfxBackend;
}


LoadMeshSystemClient::~LoadMeshSystemClient()
{
}

void LoadMeshSystemClient::initialize()
{

}

vector<ModelResource*>* LoadMeshSystemClient::createModels( const string& p_filename, const string& p_filePath, bool p_isPrimitive )
{
	return m_gfxBackend->getGfxWrapper()->createModelsFromFile(p_filename,&p_filePath,p_isPrimitive);
}

void LoadMeshSystemClient::setUpRootCollision(Entity* p_entity, ModelResource* p_modelResource)
{
	MeshOffsetTransform* offset = static_cast<MeshOffsetTransform*>(p_entity->getComponent(ComponentType::MeshOffsetTransform));
	if (!offset)
	{
		offset = new MeshOffsetTransform(p_modelResource->meshHeader.transform);
		p_entity->addComponent(ComponentType::MeshOffsetTransform, offset);
	}

	//Setup bounding sphere for frustum culling
	AglBoundingSphere sphere = p_modelResource->meshHeader.boundingSphere;

	//Correct? No? Seems like this should not be done
	//sphere.position.transform(offset->offset.inverse());

	BoundingSphere* bs = new BoundingSphere(sphere);
	p_entity->addComponent(ComponentType::BoundingSphere, bs);

	BoundingBox* bb = new BoundingBox(p_modelResource->meshHeader.minimumOBB);
	p_entity->addComponent(ComponentType::BoundingBox, bb);
}

void LoadMeshSystemClient::setUpChildCollision( Entity* p_entity, 
											   ModelResource* p_modelResource, 
											   BodyInitData* p_rootRigidBodyData, 
											   PhysicsBody* p_rootPhysicsBody, 
											   AglMatrix& baseTransform )
{
	MeshOffsetTransform* offset = static_cast<MeshOffsetTransform*>(p_entity->getComponent(ComponentType::MeshOffsetTransform));
	if (!offset)
	{
		offset = new MeshOffsetTransform(p_modelResource->meshHeader.transform);
		p_entity->addComponent(ComponentType::MeshOffsetTransform, offset);
	}

	//Setup bounding sphere for frustum culling
	AglBoundingSphere sphere = p_modelResource->meshHeader.boundingSphere;

	//Correct? No? Seems like this should not be done
	//sphere.position.transform(offset->offset.inverse());

	BoundingSphere* bs = new BoundingSphere(sphere);
	p_entity->addComponent(ComponentType::BoundingSphere, bs);

	BoundingBox* bb = new BoundingBox(p_modelResource->meshHeader.minimumOBB);
	p_entity->addComponent(ComponentType::BoundingBox, bb);
}

void LoadMeshSystemClient::setUpParticles( Entity* p_entity, ModelResource* p_modelResource )
{
	if (!p_modelResource->particleSystems.m_collection.empty())
	{
		ParticleSystemsComponent* particleComp = static_cast<ParticleSystemsComponent*>(
			p_entity->getComponent( ComponentType::ParticleSystemsComponent ) );

		if( particleComp == NULL ) {
			particleComp = new ParticleSystemsComponent();
			p_entity->addComponent( particleComp );
		}

		particleComp->addParticleSystemInstructions( p_modelResource->particleSystems );
	}
}

void LoadMeshSystemClient::setUpAnimation(Entity* p_entity, ModelResource* p_modelResource)
{
	if (p_modelResource->scene)
	{
		vector<AglSkeletonMapping*> sm = p_modelResource->scene->getSkeletonMappings();

		int index = -1;
		for (unsigned int i = 0; i < sm.size(); i++)
		{
			if (sm[i]->getHeader().meshID == p_modelResource->meshHeader.id)
				index = i;
		}

		if (index >= 0)
		{
			SkeletalAnimation* anim = static_cast<SkeletalAnimation*>(p_entity->getComponent(ComponentType::SkeletalAnimation));
			if (!anim)
			{
				anim = new SkeletalAnimation(0, p_modelResource->scene, p_modelResource->meshHeader.transform);
				p_entity->addComponent(anim);
				p_entity->applyComponentChanges();
			}
			else
			{
				anim->m_time = 0;
				anim->m_scene = p_modelResource->scene;
				anim->m_offset = p_modelResource->meshHeader.transform;
			}
		}
	}
}


void LoadMeshSystemClient::setUpLights( Entity* p_entity, ModelResource* p_modelResource )
{
	vector<LightCreationData>* lights= &(p_modelResource->lightCollection.m_collection);
	if (!lights->empty())
	{
		LightsComponent* component = new LightsComponent();
		for (unsigned int i=0;i<lights->size();i++)
		{
			// This'll be fun		
			LightCreationData* source = &(*lights)[i];
			Light light;
			TransformComponents transformHelper;
			transformHelper.scale = AglVector3(source->range,source->range,source->range);
			transformHelper.rotation = source->transform.GetRotation();
			transformHelper.translation = source->transform.GetTranslation();
			light.offsetMat = transformHelper.toMatrix();
			AglVector3 forward = source->transform.GetForward();
			light.instanceData.lightDir[0] = forward.x;
			light.instanceData.lightDir[1] = forward.y;
			light.instanceData.lightDir[2] = forward.z;
			light.instanceData.color[0] = source->diffuse.x;
			light.instanceData.color[1] = source->diffuse.y;
			light.instanceData.color[2] = source->diffuse.z;
			//light.instanceData.specular[3] = source->gloss;
			if (source->type==LightCreationData::POINT)
				light.instanceData.type = LightTypes::E_LightTypes_POINT;
			else if (source->type==LightCreationData::SPOT)
				light.instanceData.type = LightTypes::E_LightTypes_SPOT;
			else
				light.instanceData.type = LightTypes::E_LightTypes_DIRECTIONAL;
			light.instanceData.range = source->range;
			light.instanceData.attenuation[0] = source->attenuation.x;
			light.instanceData.attenuation[1] = source->attenuation.y;
			light.instanceData.attenuation[2] = source->attenuation.z;
			light.instanceData.lightEnergy = source->power;
			component->addLight(light);
		}
		p_entity->addComponent( ComponentType::LightsComponent, component );
	}
}



void LoadMeshSystemClient::setUpAmbientRangeSound( Entity* p_entity, ModelResource* p_modelResource )
{
	if (!p_modelResource->ambientRangeSoundCollection.m_collection.empty())
	{
		SoundComponent* component = new SoundComponent();
		for (unsigned int i = 0; i < p_modelResource->ambientRangeSoundCollection.m_collection.size(); i++)
		{
			AmbientRangeSoundCreationData* ardat = &p_modelResource->ambientRangeSoundCollection.m_collection[i];
			AudioHeader* ar = new AudioHeader(AudioHeader::AMBIENTRANGE,ardat->filename);
			ar->file = ardat->filename+".wav";
			ar->path = SOUNDEFFECTPATH;
			ar->playInterval = AudioHeader::FOREVER;
			ar->playingState = AudioHeader::PLAY;
			ar->queuedPlayingState = AudioHeader::PLAY;
			ar->sourceChannels = ardat->channels;
			ar->pos = ardat->transform.GetTranslation();
			ar->volume = ardat->volume;
			ar->minRange = ardat->minRange;
			ar->maxRange = ardat->maxRange;
			component->addAudioHeader(ar);
		}
		p_entity->addComponent( ComponentType::SoundComponent, component );
	}
}
