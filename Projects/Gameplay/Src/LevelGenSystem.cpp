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

LevelGenSystem::LevelGenSystem() 
	: EntitySystem(SystemType::LevelGenSystem)
{
	m_invalidAttachment = new Transform();
	m_rootTransform = nullptr;
}

LevelGenSystem::~LevelGenSystem()
{
	if (m_rootTransform)
		deleteTransformNodeRecursive(m_rootTransform);

	delete m_invalidAttachment;
}

void LevelGenSystem::initialize()
{
	graphicsBackend = static_cast<GraphicsBackendSystem*>(
						m_world->getSystem(SystemType::GraphicsBackendSystem));
}

void LevelGenSystem::run()
{
	srand(time(NULL));
	generateLevelPieces(2);
	//debugPrintTransformTree();
}

TransformNode* LevelGenSystem::createTransformNodeFromType( int p_type )
{
	TransformNode* node = new TransformNode();
	
	//int nrOfAttachments = 2; // TODO: Fetch transform data using p_type

	//node->parent	= nullptr;
	//node->transform = new Transform();
	//node->children.resize(nrOfAttachments);
	//// Init to null
	//memset(&node->children[0], 0, sizeof(TransformNode*) * nrOfAttachments);
	return node;
}

void LevelGenSystem::deleteTransformNodeRecursive( TransformNode* p_node )
{
	//for (int i = p_node->children.size() - 1; i >= 0; i--)
	//{
	//	if (p_node->children[i])
	//		deleteTransformNodeRecursive(p_node->children[i]);
	//}
	delete p_node;
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
										AglVector3::one());
	// Create the entity and specify a mesh for it
	createAndAddEntity(m_pieceTypes[0].m_meshId, transform);
	// Create the level piece to use later
	LevelPiece* piece = new LevelPiece( &m_pieceTypes[0], &m_meshHeaders[0], transform);

	// The first time, this vector will only contain the initial piece.
	vector<LevelPiece*> pieces;
	pieces.push_back(piece);

	//	AglMatrix compMatrix(m_pieceTypes[0].m_collection[i].transform);

	//	
	//	Component*	component	= new Transform(transform->getTranslation() + 
	//											compMatrix.GetTranslation(), 
	//											AglQuaternion::identity(),
	//											AglVector3::one());
	
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
			// Find a random piece type to use
			int pieceType = getRandomPieceType();
			// Create a level piece
			//LevelPiece* piece = new LevelPiece( &m_pieceTypes[pieceType], 
			//									 &m_meshHeaders[pieceType], 
			//									 new Transform() );
			// Debug: Create all pieces in order using second piece type!
			LevelPiece* piece = new LevelPiece( &m_pieceTypes[0], 
												&m_meshHeaders[0],
												new Transform() );
			
			int slot = popIntVector(freeConnectSlots);
			piece->connectTo(p_targetPiece, slot);
			//piece->connectTo(p_targetPiece, i);

			// DEBUG: Attach a cube between the connections!
			// Reminder that fetching of sprint 4 needs to be done to get new help methods
			//AglVector3 pos, scale;
			//AglQuaternion rot;

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
		}
	}

}

void LevelGenSystem::connectPieces( Transform* p_sourcePiece, Transform* p_sourceConnector,
								   Transform* p_otherPiece, Transform* p_otherConnector )
{
	// NOTE: Currently assuming that all transform info is in world space.
	// Second NOTE: p_sourcePiece and p_sourceConnector corresponds to "myPipe" and
	// "myConnector"
	// Third NOTE: This code is not done and is subjected to change

	// Reposition connection to world origo + offset from parent
	p_sourceConnector->setTranslation( p_sourceConnector->getTranslation()
										- p_sourcePiece->getTranslation() );
	// Set source piece temporarily to origo
	p_sourcePiece->setTranslation( AglVector3::zero() );
	// Now assuming that the sourcePiece is a child of the sourceConnector
	// Reposition sourceConnector on otherConnector. Don't forget reposition sourcePiece.
	p_sourceConnector->setTranslation( p_otherConnector->getTranslation() );
	p_sourcePiece->setTranslation( p_otherConnector->getTranslation() );

	// Now rotate sourceConnector. Don't forget to rotate sourcePiece.
	// myConnector.localRotation = Quaternion.LookRotation(-otherConnector.forward);
	// myConnector.RotateAround(myConnector.forward, Random.Range(0.0f, 2 * Mathf.PI)); // optional, rotate random along forward

	// Reset source piece

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

void LevelGenSystem::debugPrintTransformTree()
{
	stringstream ss;
	ss << "Level tree hierarchy:\n";

	debugPrintTransformNode(m_rootTransform, ss, 0);
	DEBUGPRINT((ss.str().c_str()));
}

void LevelGenSystem::debugPrintTransformNode( TransformNode* p_node, 
											 stringstream& p_stream,
											 int currentDepth)
{
//	for (int i = 0; i < currentDepth; i++)
//		p_stream << '\t';
//
//	if (p_node)
//	{
//		p_stream << "-> " << p_node->transform << '\n';
//		currentDepth++;
//		for (int i = 0; i < p_node->children.size(); i++)
//			debugPrintTransformNode(p_node->children[i], p_stream, currentDepth);
//	}
//	else
//		p_stream << "-> " << "(empty)" << '\n';
}

void LevelGenSystem::setPieceTypes( vector<ConnectionPointCollection> p_pieceTypes,
									vector<AglMeshHeader> p_aglMeshHeaders)
{
	m_pieceTypes	= p_pieceTypes;
	m_meshHeaders	= p_aglMeshHeaders;
}
