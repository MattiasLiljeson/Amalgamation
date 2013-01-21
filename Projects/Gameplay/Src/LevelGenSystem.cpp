#include "LevelGenSystem.h"
#include "Transform.h"
#include "RenderInfo.h"
#include <AglVector3.h>
#include <sstream>
#include <DebugUtil.h>
#include "GraphicsBackendSystem.h"
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

LevelGenSystem::LevelGenSystem(GraphicsBackendSystem* p_graphicsBackend, TcpServer* p_server) 
	: EntitySystem(SystemType::LevelGenSystem)
{
	m_graphicsBackend = p_graphicsBackend;
	m_server = p_server;
}

LevelGenSystem::~LevelGenSystem()
{
	// TODO: delete generated pieces in order to prevent memory leaks!
	for (int i = 0; i < m_generatedPieces.size(); i++)
	{
		delete m_generatedPieces[i];
	}
	m_generatedPieces.clear();

	for (int i = 0; i < m_modelResources.size(); i++)
		delete m_modelResources[i];

	m_modelResources.clear();
}

void LevelGenSystem::initialize()
{
	for (int i = 0; i < m_modelFileMapping.getModelFileCount() - 1; i++)
	{
		string modelName = m_modelFileMapping.getModelFileName(i);

		if ( !m_server)
		{
			m_graphicsBackend->loadSingleMeshFromFile( modelName,
			&TESTMODELPATH);
		}

		auto resourcesFromModel = m_unmanagedModelFactory.createModelResources(modelName,
																		&TESTMODELPATH);
		m_modelResources.push_back( resourcesFromModel->at(0) );

		delete resourcesFromModel;
	}
}

void LevelGenSystem::run()
{
	srand(time(NULL));
	generateLevelPieces(5);
}

void LevelGenSystem::generateLevelPieces( int p_maxDepth )
{
	// Create a initial piece.
	Transform* transform = new Transform(AglVector3(15, 20, 15), 
										AglQuaternion::identity(),
										AglVector3::one() * 2.0f);
	
	// Create the level piece to use later
	//LevelPiece* piece = new LevelPiece( &m_pieceTypes[0], &m_meshHeaders[0], transform);
	LevelPiece* piece = new LevelPiece( 0, m_modelResources[0], transform);

	// Create the entity and specify a mesh for it
	createAndAddEntity(0, transform);

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
		for (int i = 0; i < pieces.size(); i++)
			generatePiecesOnPiece(pieces[i], temps);

		// Creates a piece entity and adds it to the world
		for (int i = 0; i < temps.size(); i++)
		{
			createAndAddEntity( temps[i]->getTypeId(), temps[i]->getTransform());
		}

		// For the next iteration round
		pieces = vector<LevelPiece*>(temps);
	}
}

void LevelGenSystem::createAndAddEntity( int p_type, Transform* p_transform )
{
	Entity* entity = m_world->createEntity();

	entity->addComponent(ComponentType::Transform, p_transform);
	entity->addComponent(ComponentType::StaticProp, new StaticProp(p_type, true));

	if ( !m_server )
	{
		entity->addComponent(ComponentType::RenderInfo, 
			new RenderInfo( getMeshFromPieceType( p_type ) ));
	}

	// TODO: There needs to be added more components to the entity before it is added to
	// the world
	m_world->addEntity(entity);
}

void LevelGenSystem::generatePiecesOnPiece( LevelPiece* p_targetPiece, 
										   vector<LevelPiece*>& out_pieces )
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
			int pieceType = m_modelFileMapping.getRandomPieceId();
			
			// Create a level piece
			//LevelPiece* piece = new LevelPiece( &m_pieceTypes[pieceType], 
			//									 &m_meshHeaders[pieceType], 
			//									 new Transform() );
			// Debug: Create all pieces in order using second piece type!
			//LevelPiece* piece = new LevelPiece( &m_pieceTypes[pieceType], 
			//									&m_meshHeaders[pieceType],
			//									new Transform() );
			LevelPiece* piece = new LevelPiece( pieceType, m_modelResources[pieceType],
												new Transform() );

			int slot = popIntVector(freeConnectSlots);
			piece->connectTo(p_targetPiece, slot);
			//piece->connectTo(p_targetPiece, i);

			// Verify that it isn't colliding with previous level pieces.
			bool colliding = false;
			for (int i = 0; i < m_generatedPieces.size(); i++)
			{
				if (AglCollision::isColliding( piece->getBoundingBox(),
					m_generatedPieces[i]->getBoundingBox()) && 
					piece->getChild(0) != m_generatedPieces[i]->getTransform() )
				{
					colliding = true;
					break;
				}
			}

			if (colliding)
			{
				// Remove the connected component.
				p_targetPiece->setChild(slot, NULL);
				piece->deleteMainTransform();
				delete piece;
			}			
			else
			{
				// DEBUG: Attach a cube between the connections!
				AglMatrix targetConnectorMatrix = p_targetPiece->getConnectionPointMatrix(slot);
				AglMatrix thisConnectorMatrix	= piece->getConnectionPointMatrix(0);

				int endPlugId = m_modelFileMapping.getEndPlugId();

				createAndAddEntity(endPlugId, new Transform(targetConnectorMatrix.GetTranslation(), 
					targetConnectorMatrix.GetRotation(),
					AglVector3::one() * 0.5f));
				createAndAddEntity(endPlugId, new Transform(thisConnectorMatrix.GetTranslation(), 
					thisConnectorMatrix.GetRotation(),
					AglVector3::one() * 0.5f));

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

	createAndAddEntity( m_modelFileMapping.getEndPlugId(), p_atConnector);
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

int LevelGenSystem::getMeshFromPieceType( int p_typeId ) const
{
	return m_graphicsBackend->getMeshId( m_modelFileMapping.getModelFileName( p_typeId ) );
}
