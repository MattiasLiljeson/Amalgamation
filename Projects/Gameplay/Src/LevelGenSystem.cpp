#define FORCE_VS_DBG_OUTPUT

#include "LevelGenSystem.h"
#include "Transform.h"
#include "RenderInfo.h"
#include <AglVector3.h>
#include <sstream>
#include <DebugUtil.h>
#include "LevelPiece.h"
#include <cstdlib>
#include <ctime>
#include <AglCollision.h>
#include "StaticProp.h"
#include <TcpServer.h>
#include "EntityCreationPacket.h"
#include "EntityType.h"
#include <Globals.h>
#include <ModelResource.h>
#include <string>
#include "GameplayTags.h"
#include "BodyInitData.h"
#include "PhysicsBody.h"
#include "BoundingVolumeInitData.h"
#include "LoadMeshSystemServer.h"
#include "EntityFactory.h"
#include "LevelInfo.h"
#include "LevelPieceRoot.h"

LevelGenSystem::LevelGenSystem(TcpServer* p_server) 
	: EntitySystem(SystemType::LevelGenSystem, 1, ComponentType::LevelInfo)
{
	m_server = p_server;

	m_worldMin = AglVector3((float)INT_MAX, (float)INT_MAX, (float)INT_MAX);
	m_worldMax = AglVector3((float)INT_MIN, (float)INT_MIN, (float)INT_MIN);

	m_entityFactory			= NULL;
	m_levelInfo				= NULL;
	m_endPlugModelResource	= NULL;
	m_readyToRun			= false;
	m_hasGeneratedLevel		= false;
}

LevelGenSystem::~LevelGenSystem()
{
	clearGeneratedData();
}

void LevelGenSystem::initialize()
{
	m_entityFactory = static_cast<EntityFactory*>(
		m_world->getSystem(SystemType::EntityFactory));

	preloadLevelGenRecipeEntity( LEVELPIECESPATH + "LevelGenServer.asd");
}

void LevelGenSystem::calculatePieceCollision( vector<ModelResource*>* p_pieceMesh )
{
	// Index 0 always contains the root.
	if (p_pieceMesh->size() > 0)
	{
		AglBoundingSphere boundingSphere = p_pieceMesh->at(1)->meshHeader.boundingSphere;
		AglMatrix mat ;

		for (unsigned int i = 2; i < p_pieceMesh->size(); i++)
		{
			ModelResource* resource = p_pieceMesh->at(i);
			AglBoundingSphere nextBoundingSphere = resource->meshHeader.boundingSphere;
			
			AglVector3 pos = (resource->meshHeader.transform * resource->transform).GetTranslation();
			nextBoundingSphere.position = pos;

			nextBoundingSphere.radius = abs(nextBoundingSphere.radius);
			boundingSphere = AglBoundingSphere::mergeSpheres(boundingSphere, 
				nextBoundingSphere);
		}
		string logtext = p_pieceMesh->at(0)->name + " : r=" + toString(boundingSphere.radius) + "\n";
		DEBUGPRINT( (logtext.c_str()) );

		p_pieceMesh->at(0)->meshHeader.boundingSphere = boundingSphere;
	}
}

void LevelGenSystem::preloadLevelGenRecipeEntity(const string& p_filePath)
{
	string recipeName;
	m_entityFactory->readAssemblageFile(p_filePath, &recipeName);
	Entity* e = m_entityFactory->entityFromRecipe(recipeName);
	m_world->addEntity(e);
}

void LevelGenSystem::inserted( Entity* p_entity )
{
	m_levelInfo			= static_cast<LevelInfo*>(p_entity->getComponent(ComponentType::LevelInfo));
	m_startTransform	= static_cast<Transform*>(p_entity->getComponent(ComponentType::Transform))->getMatrix();

	auto loadMeshSys = static_cast<LoadMeshSystemServer*>(
		m_world->getSystem(SystemType::LoadMeshSystem));

	vector<LevelPieceFileData*> fileData = m_levelInfo->getFileData();
	for (int i = 0; i < fileData.size(); i++)
	{
		string modelName = fileData[i]->modelFileName;	

		// Preload chamber models here. This is required, and must be done before an
		// entity is created.
		auto resourcesFromModel = loadMeshSys->createModels(modelName,
			MODELPATH, false);

		ModelResource* rootResource = resourcesFromModel->at(0);

		// Preload the chamber assemblages.
		m_entityFactory->readAssemblageFile(LEVELPIECESPATH + fileData[i]->assemblageFileName,
											&fileData[i]->assemblageName);

		// Calculate the entire chamber's collision sphere.
		// NOTE: Uncertain whether or not this value should be multiplied by 2 or not
		// before being used.
		calculatePieceCollision(resourcesFromModel);

		m_modelResources.push_back( rootResource );

		//delete resourcesFromModel;
	}
	LevelPieceFileData* endPlug = m_levelInfo->getEndPlugFileData();
	m_entityFactory->readAssemblageFile(LEVELPIECESPATH + endPlug->assemblageFileName,
		&endPlug->assemblageName);
	auto resourcesFromModel = loadMeshSys->createModels(endPlug->modelFileName,
		MODELPATH, false);
	m_endPlugModelResource = resourcesFromModel->at(0);

	m_readyToRun = true;
	// Temp: This is to make sure the system works.
	run();

	//m_world->deleteEntity(p_entity);
}

void LevelGenSystem::removed( Entity* p_entity )
{
	m_levelInfo = NULL;
}

void LevelGenSystem::clearGeneratedData()
{
	// Destroy generated pieces.
	for (unsigned int i = 0; i < m_generatedPieces.size(); i++)
	{
		delete m_generatedPieces[i];
	}
	m_generatedPieces.clear();
	// Clear model resources (Don't delete, since the level gen doesn't have ownership)
	m_modelResources.clear();

	// There's still data that exists, such as init data. These should not be destroyed.
}

void LevelGenSystem::run()
{
	if (m_readyToRun)
	{
		m_hasGeneratedLevel = false;
		srand(static_cast<unsigned int>(time(NULL)));
		generateLevelPieces(m_levelInfo->getBranchCount(), m_levelInfo->doRandomStartRotation());
		createLevelEntities();
		m_hasGeneratedLevel = true;
	}
	else
	{
		DEBUGPRINT(("Warning: LevelGenSystem::run was called, but the system is not ready to run yet.\n"));
	}
}

void LevelGenSystem::generateLevelPieces( int p_maxDepth, bool p_doRandomStartRotation)
{
	// Creates the first entity.
	AglQuaternion quart;
	if (p_doRandomStartRotation)
		quart = AglQuaternion::constructFromAxisAndAngle(AglVector3::forward(),
														(rand() % 360) * 3.1415f / 180.0f);
	else
		quart = AglQuaternion::identity();

	// Create a initial piece.
	Transform* transform = new Transform(m_startTransform);
	transform->setRotation(quart);

	// Create the level piece to use later
	int id = m_levelInfo->getStartFileData()->id;
	LevelPiece* piece = new LevelPiece(id, m_modelResources[id], transform, 0);
	piece->setPieceId(0);

	// The first time, this vector will only contain the initial piece.
	vector<LevelPiece*> pieces;
	pieces.push_back(piece);
	m_generatedPieces.push_back(piece);

	// The algorithm generates pieces outwards based on a so called depth. For each depth
	// the pieces from the previous depth are used to create and connect new pieces.
	for (int currentDepth = 0; currentDepth < p_maxDepth; currentDepth++)
	{
		// Stores created pieces temporarily
		vector<LevelPiece*> temps;
		for (unsigned int i = 0; i < pieces.size(); i++)
			generatePiecesOnPiece(pieces[i], temps, currentDepth+1);

		pieces = vector<LevelPiece*>(temps);
	}
}

Entity* LevelGenSystem::createEntity( LevelPiece* p_piece)
{
	Entity* entity = m_entityFactory->entityFromRecipe( 
		m_levelInfo->getFileDataFromId( p_piece->getTypeId() )->assemblageName );
	
	if (!entity)
	{
		DEBUGWARNING(("LevelGenSystem Warning: Unable to create the specified level piece entity!"));
	}
	else
	{
		auto transform = static_cast<Transform*>(
			entity->getComponent(ComponentType::Transform));

		if (!transform)
		{
			transform = new Transform();
			entity->addComponent( transform );
			transform->setTranslation( p_piece->getTransform()->getTranslation() );
			transform->setRotation( p_piece->getTransform()->getRotation() );
		}
		// ISSUE?!? Scale can not be transferred from the piece.
		transform->setTranslation( p_piece->getTransform()->getTranslation() );
		transform->setRotation( p_piece->getTransform()->getRotation() );

		entity->addComponent(new StaticProp(p_piece->getTypeId(), true));

		auto pieceRoot = static_cast<LevelPieceRoot*>(
			entity->getComponent(ComponentType::LevelPieceRoot));

		pieceRoot->pieceId = p_piece->getPieceId();
	}

	return entity;
}

Entity* LevelGenSystem::createDebugSphereEntity( LevelPiece* p_piece )
{
	/* DEBUG */
	//AglBoundingSphere boundingSphere = p_piece->getBoundingSphere();
	//AglVector3 scale(boundingSphere.radius, boundingSphere.radius, boundingSphere.radius);
	////scale *= 2;

	//Entity* e = m_world->createEntity();
	//e->addComponent(new Transform(boundingSphere.position, AglQuaternion::identity(),
	//	scale));
	//e->addComponent(new StaticProp(1));
	//m_world->addEntity(e);
	//return e;
	/* END OF DEBUG */
	return NULL;
}

void LevelGenSystem::generatePiecesOnPiece( LevelPiece* p_targetPiece, 
										   vector<LevelPiece*>& out_pieces, int p_generation )
{
	vector<int> freeConnectSlots = p_targetPiece->findFreeConnectionPointSlots();

	// Check that there's any available pipes
	if ( !freeConnectSlots.empty() )
	{
		// Prepare and connect newly created transform pieces to the target.
		int maxPiecesCount = freeConnectSlots.size();
		for (int i = 0; i < maxPiecesCount; i++)
		{
			// TODO: ??? Currently, if the randomized piece type collides with other pieces
			// on the desired position, it will set that corresponding slot on the target
			// to be occupied. For better generating, a new piece type should be selected
			// and collision tested until there's no valid piece type at all, before
			// setting the target connector to be occupied.

			// Find a random piece type to use
			int pieceType = m_levelInfo->getRandomFileData()->id;
			
			// Create a level piece
			LevelPiece* piece = new LevelPiece( pieceType, m_modelResources[pieceType],
												new Transform(), p_generation);

			int slot = popIntVector(freeConnectSlots);
			piece->connectTo(p_targetPiece, slot);

			// Verify that it isn't colliding with previous level pieces.
			bool colliding = false;
			for (unsigned int i = 0; i < m_generatedPieces.size(); i++)
			{
				if (AglCollision::isColliding( piece->getBoundingSphere(),
					m_generatedPieces[i]->getBoundingSphere()) && 
					piece->getChild(0) != m_generatedPieces[i]->getTransform() )
				{
					DEBUGPRINT(("Collision between chambers detected. This means a level plug has been created instead.\n"));
					colliding = true;
					break;
				}
			}

			if (colliding)
			{
				// Remove the connected component.
				p_targetPiece->setChild(slot, NULL);
				delete piece;
			}			
			else
			{
				AglOBB obb = piece->getBoundingBox();
				updateWorldMinMax(obb);

				out_pieces.push_back(piece);
				piece->setPieceId(m_generatedPieces.size());
				m_generatedPieces.push_back(piece);
			}
		}
	}

}

int LevelGenSystem::popIntVector( vector<int>& p_vector )
{
	int i = -1;
	if ( !p_vector.empty() )
	{
		i = p_vector.back();
		p_vector.pop_back();
	}
	return i;
}

void LevelGenSystem::createLevelEntities()
{
	for (unsigned int i = 0; i < m_generatedPieces.size(); i++)
	{
		Entity* e = createEntity(m_generatedPieces[i]);
		m_world->addEntity(e);
		addEndPlugs(m_generatedPieces[i]);
		//e = createDebugSphereEntity(m_generatedPieces[i]);
		//m_world->addEntity(e);
	}
	DEBUGPRINT(((toString(m_generatedPieces.size()) + " chambers generated.\n").c_str()));
}

const AglVector3&  LevelGenSystem::getWorldMin() const
{
	return m_worldMin;
}

const AglVector3&  LevelGenSystem::getWorldMax() const
{
	return m_worldMax;
}
vector<LevelPiece*> LevelGenSystem::getGeneratedPieces() const
{
	return m_generatedPieces;
}

void LevelGenSystem::updateWorldMinMax( AglOBB& boundingVolume )
{
	auto corners = boundingVolume.getCorners();

	for (unsigned int i = 0; i < corners.size(); i++)
	{
		m_worldMin = AglVector3::minOf(m_worldMin, corners[i]);
		m_worldMax = AglVector3::maxOf(m_worldMax, corners[i]);
	}
}

Entity* LevelGenSystem::addEndPlug( Transform* p_atConnector )
{
	Entity* entity = m_entityFactory->entityFromRecipe( 
		m_levelInfo->getEndPlugFileData()->assemblageName );

	auto transform = static_cast<Transform*>(
		entity->getComponent(ComponentType::Transform));

	AglVector3 tempScale = AglVector3::one();
	if (!transform)
	{
		transform = new Transform();
		entity->addComponent( transform );
	}
	else
	{
		tempScale = transform->getScale();
	}

	// Set proper rotation and scale of the plug piece
	transform->setMatrix(p_atConnector->getMatrix());
	transform->setForwardDirection( -transform->getForward() );
	transform->setScale(tempScale);

	entity->addComponent(new StaticProp(m_levelInfo->getEndPlugFileData()->id, true));

	// Manipulate the BodyInitData translation info to match the transform, and add a proper
	// model resource.
	auto bodyInitData = static_cast<BodyInitData*>(entity->getComponent(ComponentType::BodyInitData));
	if (bodyInitData)
	{
		bodyInitData->m_position		= transform->getTranslation();
		bodyInitData->m_orientation		= transform->getRotation();
		bodyInitData->m_scale			= transform->getScale();
		bodyInitData->m_modelResource	= m_endPlugModelResource;
	}
	return entity;
}

void LevelGenSystem::addEndPlugs(LevelPiece* p_atPiece)
{
	for (unsigned int i = 0; i < p_atPiece->getMaxChildCount(); i++)
	{
		if (!p_atPiece->isChildSlotOccupied(i))
		{
			Entity* plug = addEndPlug(&p_atPiece->getConnectionPoint(i));
			m_world->addEntity(plug);
			DEBUGPRINT(("Added end plug!\n"));
		}
	}
}

int LevelGenSystem::getGeneratedPiecesCount() const
{
	return m_generatedPieces.size();
}





