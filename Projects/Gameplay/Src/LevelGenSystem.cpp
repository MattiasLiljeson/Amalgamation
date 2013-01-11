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

LevelGenSystem::LevelGenSystem() 
	: EntitySystem(SystemType::LevelGenSystem)
{
}

LevelGenSystem::~LevelGenSystem()
{
	// TODO: delete generated pieces in order to prevent memory leaks!
}

void LevelGenSystem::initialize()
{
	graphicsBackend = static_cast<GraphicsBackendSystem*>(
						m_world->getSystem(SystemType::GraphicsBackendSystem));
}

void LevelGenSystem::run()
{
	srand(time(NULL));
	generateLevelPieces(5);
}

int LevelGenSystem::getRandomPieceType()
{
	return rand() % m_pieceTypes.size();
}


void LevelGenSystem::generateLevelPieces( int p_maxDepth )
{
	// Create a initial piece.
	Transform* transform = new Transform(AglVector3(15, 20, 15), 
										AglQuaternion::identity(),
										AglVector3::one() * 2.0f);
	// Create the entity and specify a mesh for it
	createAndAddEntity(m_pieceTypes[0].m_meshId, transform);
	// Create the level piece to use later
	LevelPiece* piece = new LevelPiece( &m_pieceTypes[0], &m_meshHeaders[0], transform);

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
			createAndAddEntity(temps[i]->getMeshId(), temps[i]->getTransform());
		
		// For the next iteration round
		pieces = vector<LevelPiece*>(temps);
	}
}

void LevelGenSystem::createAndAddEntity( int p_type, Transform* p_transform )
{
	Entity* entity = m_world->createEntity();

	entity->addComponent(ComponentType::Transform, p_transform);
	entity->addComponent(ComponentType::RenderInfo, new RenderInfo(p_type) );
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
		vector<LevelPiece*> generatedPieces(freeConnectSlots.size());

		//generatedPieces.resize(2);

		for (int i = 0; i < generatedPieces.size(); i++)
		{
			// TODO: ? Currently, if the randomized piece type collides with other pieces
			// on the desired position, it will set that corresponding slot on the target
			// to be occupied. For better generating, a new piece type should be selected
			// and collision tested until there's no valid piece type at all, before
			// setting the target connector to be occupied.

			// Find a random piece type to use
			int pieceType = getRandomPieceType();
			
			// Create a level piece
			//LevelPiece* piece = new LevelPiece( &m_pieceTypes[pieceType], 
			//									 &m_meshHeaders[pieceType], 
			//									 new Transform() );
			// Debug: Create all pieces in order using second piece type!
			LevelPiece* piece = new LevelPiece( &m_pieceTypes[pieceType], 
												&m_meshHeaders[pieceType],
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
				delete piece;
			}			
			else
			{
				// DEBUG: Attach a cube between the connections!

				AglMatrix targetConnectorMatrix = p_targetPiece->getConnectionPointMatrix(slot);
				AglMatrix thisConnectorMatrix	= piece->getConnectionPointMatrix(0);

				//piece->getConnectionPointMatrix(0, Space_GLOBAL).toComponents(scale, rot, pos);
				createAndAddEntity(0, new Transform(targetConnectorMatrix.GetTranslation(), 
													targetConnectorMatrix.GetRotation(),
													AglVector3::one() * 0.5f));
				//p_targetPiece->getLocalConnectionPointMatrix(i, Space_GLOBAL).toComponents(scale, rot, pos);
				createAndAddEntity(0, new Transform(thisConnectorMatrix.GetTranslation(), 
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
	int cubeId = graphicsBackend->getMeshId("P_cube");

	createAndAddEntity(cubeId, p_atConnector);
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

void LevelGenSystem::setPieceTypes( vector<ConnectionPointCollection> p_pieceTypes,
									vector<AglMeshHeader> p_aglMeshHeaders)
{
	m_pieceTypes	= p_pieceTypes;
	m_meshHeaders	= p_aglMeshHeaders;
}
