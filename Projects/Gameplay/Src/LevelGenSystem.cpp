#include "LevelGenSystem.h"
#include "Transform.h"
#include <AglVector3.h>
#include <sstream>
#include <DebugUtil.h>

LevelGenSystem::LevelGenSystem() : EntitySystem(SystemType::LevelGenSystem)
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
	generateLevelPieces(4);
	debugPrintTransformTree();
}


TransformNode* LevelGenSystem::createTransformNodeFromType( int p_type )
{
	TransformNode* node = new TransformNode();
	
	int nrOfAttachments = 2; // TODO: Fetch transform data using p_type

	node->parent	= nullptr;
	node->transform = new Transform();
	node->children.resize(nrOfAttachments);
	// Init to null
	memset(&node->children[0], 0, sizeof(TransformNode*) * nrOfAttachments);
	return node;
}

void LevelGenSystem::deleteTransformNodeRecursive( TransformNode* p_node )
{
	for (int i = p_node->children.size() - 1; i >= 0; i--)
	{
		if (p_node->children[i])
			deleteTransformNodeRecursive(p_node->children[i]);
	}
	delete p_node;
}



void LevelGenSystem::generateLevelPieces( int p_maxDepth )
{
	// Create the first entity piece
	Entity* entity = m_world->createEntity();

	// Create root transform
	// TODO: Insert a randomly chosen identity, instead of 0
	TransformNode* transformNode = createTransformNodeFromType( 0 ); 
	Transform* transform = transformNode->transform;

	// Add root transform to the hierarchy
	// m_transformHierarchy[transform] = transformNode;

	entity->addComponent(ComponentType::Transform, transform);
	
	// TODO: There needs to be added more components to the entity before it is added to
	// the world
	m_world->addEntity(entity);

	// The first time, this vector will only contain the first root transform.
	vector<TransformNode*> pieces;
	pieces.push_back(transformNode);
	m_rootTransform = transformNode;

	for (int currentDepth = 0; currentDepth < p_maxDepth; currentDepth++)
	{
		vector<TransformNode*> temps;
		for (int i = 0; i < pieces.size(); i++)
			generatePiecesOnPiece(pieces[i], temps);

		for (int i = 0; i < temps.size(); i++)
			createAndAddEntity(0, temps[i]->transform);
		
		pieces = vector<TransformNode*>(temps);
	}
}

void LevelGenSystem::createAndAddEntity( int p_type, Transform* p_transform )
{
	Entity* entity = m_world->createEntity();

	entity->addComponent(ComponentType::Transform, p_transform);

	// TODO: There needs to be added more components to the entity before it is added to
	// the world
	m_world->addEntity(entity);
}

void LevelGenSystem::generatePiecesOnPiece( TransformNode* p_targetPiece, 
										   vector<TransformNode*>& out_pieces )
{
	vector<int> freeConnectSlots;
	getFreeConnectPointSlots(p_targetPiece, freeConnectSlots);

	// Check that there's any available pipes
	if ( !freeConnectSlots.empty() )
	{
		// Prepare and connect newly created transform pieces to the target.
		vector<TransformNode*> generatedPieces(freeConnectSlots.size());
		for (int i = 0; i < generatedPieces.size(); i++)
		{
			// NOTE: The hard-coded value 0 should be replaced by a random piece type id
			generatedPieces[i] = createTransformNodeFromType( 0 );

			// NOTE: Need to find a way to fetch the proper attachment point here.
			Transform* sourceConnector; // = getAttachmentPoint(generatedPieces[i]);

			// NOTE: This is unfinished... needs to have more insight of the structure
			// before continuing
			//Transform* otherConnector = p_targetPiece->
			//							children[popIntVector(freeConnectSlots)]->
			//							transform;

			//connectPieces(generatedPieces[i]->transform, sourceConnector, 
			//			  p_targetPiece->transform, otherConnector);


			out_pieces.push_back(generatedPieces[i]);
		}
	}

}

void LevelGenSystem::getFreeConnectPointSlots( TransformNode* p_piece, vector<int>& out_slots )
{
	// NOTE: This method is subjected to change, and is not yet finished.

	// Check for an unoccupied slot
	for (int i = 0; i < p_piece->children.size(); i++)
	{
		if ( !p_piece->children[i] )
			out_slots.push_back(i);
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
	Entity* entity = m_world->createEntity();
	Transform* endplug = new Transform(p_atConnector->getTranslation(), 
										p_atConnector->getRotation(),
										p_atConnector->getScale());

	entity->addComponent(ComponentType::Transform, endplug);

	// NOTE: Here misses some components, such as model mesh and collider, yaddayadda

	m_world->addEntity(entity);
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
	for (int i = 0; i < currentDepth; i++)
		p_stream << '\t';

	if (p_node)
	{
		p_stream << "-> " << p_node->transform << '\n';
		currentDepth++;
		for (int i = 0; i < p_node->children.size(); i++)
			debugPrintTransformNode(p_node->children[i], p_stream, currentDepth);
	}
	else
		p_stream << "-> " << "(empty)" << '\n';
}
