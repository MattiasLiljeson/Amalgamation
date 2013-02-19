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
#include "LevelPieceInfo.h"
#include "LoadMeshSystemServer.h"
#include "EntityFactory.h"
#include "LevelInfo.h"

#define FORCE_VS_DBG_OUTPUT

LevelGenSystem::LevelGenSystem(TcpServer* p_server) 
	: EntitySystem(SystemType::LevelGenSystem, 1, ComponentType::LevelInfo)
{
	m_server = p_server;

	m_worldMin = AglVector3((float)INT_MAX, (float)INT_MAX, (float)INT_MAX);
	m_worldMax = AglVector3((float)INT_MIN, (float)INT_MIN, (float)INT_MIN);

	m_entityFactory = NULL;
	m_levelInfo		= NULL;
}

LevelGenSystem::~LevelGenSystem()
{
	clearGeneratedData();
}

void LevelGenSystem::initialize()
{
	m_entityFactory = static_cast<EntityFactory*>(
		m_world->getSystem(SystemType::EntityFactory));

	// Preload entity recipes here. These are then used to create entities.
	//AssemblageHelper::E_FileStatus status = 
	//	m_entityFactory->readAssemblageFile( "Assemblages/rocksServer.asd" );
	//status = 
	//	m_entityFactory->readAssemblageFile( "Assemblages/tunnelServer.asd" );

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
		boundingSphere.radius;
		p_pieceMesh->at(0)->meshHeader.boundingSphere = boundingSphere;
	}
}

void LevelGenSystem::processEntities( const vector<Entity*>& p_entities )
{
	// Currently this piece of code is never ran.
	/*LevelPieceInfo* pieceInfo = NULL;
	Transform*		transform = NULL;
	Transform*		fetchedTransform = NULL;

	for (int i = 0; i < p_entities.size(); i++)
	{
		Entity* entity = p_entities[i];

		pieceInfo = static_cast<LevelPieceInfo*>(
			entity->getComponent(ComponentType::LevelPieceInfo));
		transform = static_cast<Transform*>(
			entity->getComponent(ComponentType::Transform));
		fetchedTransform = m_generatedPieces[pieceInfo->getPieceInstanceId()]->getTransform();

		// Update the position and rotation for now.
		transform->setTranslation(fetchedTransform->getTranslation());
		transform->setRotation(fetchedTransform->getRotation());

		entity->removeComponent(ComponentType::LevelPieceInfo);
		entity->applyComponentChanges();
	}*/
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
	m_levelInfo = static_cast<LevelInfo*>(p_entity->getComponent(ComponentType::LevelInfo));
	// TODO: parse levelInfo and generate!

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

	// Temp: This is to make sure the system works.
	srand(static_cast<unsigned int>(time(NULL)));
	generateLevelPieces(m_levelInfo->getBranchCount(), m_levelInfo->doRandomStartRotation());
	createLevelEntities();

	m_world->deleteEntity(p_entity);
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
	//srand(static_cast<unsigned int>(time(NULL)));
	//generateLevelPieces(1);
	//createLevelEntities();
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
	Transform* transform = new Transform(AglVector3(20, -20, 10), 
										quart, 
										AglVector3::one());
	
	// Create the level piece to use later
	//LevelPiece* piece = new LevelPiece( &m_pieceTypes[0], &m_meshHeaders[0], transform);
	int id = m_levelInfo->getStartFileData()->id;
	LevelPiece* piece = new LevelPiece(id, m_modelResources[id], transform, 0);

	// Create the entity and specify a mesh for it
	//createAndAddEntity(0, transform, piece->getBoundingBox());

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

		// Creates a piece entity and adds it to the world
		//for (int i = 0; i < temps.size(); i++)
		//{
		//	createAndAddEntity( temps[i]->getTypeId(), temps[i]->getTransform(),
		//						temps[i]->getBoundingBox());
		//}

		// For the next iteration round
		pieces = vector<LevelPiece*>(temps);
	}
}

Entity* LevelGenSystem::createEntity( LevelPiece* p_piece, int p_pieceInstanceId )
{
	Entity* entity = m_entityFactory->entityFromRecipe( 
		m_levelInfo->getFileDataFromId( p_piece->getTypeId() )->assemblageName );
	
	if (!entity)
	{
		DEBUGWARNING(("LevelGenSystem Warning: Unable to create the specified level piece entity!"));
	}
	else
	{
		//entity->addComponent(new LevelPieceInfo(p_pieceInstanceId));
		auto transform = static_cast<Transform*>(
			entity->getComponent(ComponentType::Transform));

		// ISSUE?!? Scale can not be transferred from the piece
		if (transform)
		{
			transform->setTranslation( p_piece->getTransform()->getTranslation() );
			transform->setRotation( p_piece->getTransform()->getRotation() );
			//transform->setMatrix( p_piece->getTransform()->getMatrix() );
		}
		else
		{	
			transform = new Transform();
			transform->setTranslation( p_piece->getTransform()->getTranslation() );
			transform->setRotation( p_piece->getTransform()->getRotation() );
			entity->addComponent( transform );
		}
		entity->addComponent(new StaticProp(p_piece->getTypeId(), true));
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
			//LevelPiece* piece = new LevelPiece( &m_pieceTypes[pieceType], 
			//									 &m_meshHeaders[pieceType], 
			//									 new Transform() );
			// Debug: Create all pieces in order using second piece type!
			//LevelPiece* piece = new LevelPiece( &m_pieceTypes[pieceType], 
			//									&m_meshHeaders[pieceType],
			//									new Transform() );
			LevelPiece* piece = new LevelPiece( pieceType, m_modelResources[pieceType],
												new Transform(), p_generation);

			int slot = popIntVector(freeConnectSlots);
			piece->connectTo(p_targetPiece, slot);
			//piece->connectTo(p_targetPiece, i);

			// Verify that it isn't colliding with previous level pieces.
			bool colliding = false;
			for (unsigned int i = 0; i < m_generatedPieces.size(); i++)
			{
				if (AglCollision::isColliding( piece->getBoundingSphere(),
					m_generatedPieces[i]->getBoundingSphere()) && 
					piece->getChild(0) != m_generatedPieces[i]->getTransform() )
				{
					DEBUGPRINT(("Collision between chambers detected. Failed to generate!"));
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
				// DEBUG: Attach a cube between the connections!
				//AglMatrix targetConnectorMatrix = p_targetPiece->getConnectionPointMatrix(slot);
				//AglMatrix thisConnectorMatrix	= piece->getConnectionPointMatrix(0);

				//int endPlugId = m_modelFileMapping.getEndPlugId();

				//createAndAddEntity(endPlugId, new Transform(targetConnectorMatrix.GetTranslation(), 
				//	targetConnectorMatrix.GetRotation(),
				//	AglVector3::one() * 0.5f), AglVector::zero());
				//createAndAddEntity(endPlugId, new Transform(thisConnectorMatrix.GetTranslation(), 
				//	thisConnectorMatrix.GetRotation(),
				//	AglVector3::one() * 0.5f), AglVector::zero());
				AglOBB obb = piece->getBoundingBox();
				updateWorldMinMax(obb);

				out_pieces.push_back(piece);
				m_generatedPieces.push_back(piece);
			}
		}
	}

}

void LevelGenSystem::addEndPlug( Transform* p_atConnector )
{
	// Connector is assumed to be in world space!
	//int cubeId = m_graphicsBackend->getMeshId("P_cube");

	//createAndAddEntity( m_modelFileMapping.getEndPlugId(), p_atConnector, 
	//	p_atConnector->getScale() * AglVector3::one());
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
		Entity* e = createEntity(m_generatedPieces[i], i);
		m_world->addEntity(e);
		e = createDebugSphereEntity(m_generatedPieces[i]);
		//m_world->addEntity(e);
	}
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


