#include "LevelGenSystem.h"
#include "Transform.h"
#include <AglVector3.h>
#include <sstream>
#include <DebugUtil.h>

LevelGenSystem::LevelGenSystem() : EntitySystem(SystemType::LevelGenSystem)
{
	m_invalidAttachment = new Transform();
}

LevelGenSystem::~LevelGenSystem()
{
	delete m_invalidAttachment;
}

void LevelGenSystem::initialize()
{
	generateLevelPieces(4);
	debugPrintTransformTree();
}

void LevelGenSystem::generateLevelPieces( int p_maxDepth )
{
	// Create the first entity piece
	Entity* entity = m_world->createEntity();
	// Create root transform
	Transform* transform = new Transform(AglVector3::zero(), AglQuaternion::identity(),
										AglVector3::one());

	entity->addComponent(ComponentType::Transform, transform);
	m_world->addEntity(entity);

	// The first time, this vector will only contain the first root transform.
	vector<Transform*> pieces;
	pieces.push_back(transform);
	m_rootTransform = transform;

	for (int currentDepth = 0; currentDepth < p_maxDepth; currentDepth++)
	{
		vector<Transform*> temps;
		for (int i = 0; i < pieces.size(); i++)
		{
			generatePiecesOnPiece(pieces[i], temps);
			temps.clear();
		}
		pieces = vector<Transform*>(temps);
	}



}

void LevelGenSystem::generatePiecesOnPiece( Transform* p_targetPiece, 
										   vector<Transform*>& out_pieces )
{
	vector<Transform*> freeAttachments;
	getFreeAttachments(p_targetPiece, freeAttachments);
	
	// Check that there's any available pipes
	if ( !freeAttachments.empty() )
	{
		// Prepare and connect newly created transform pieces to the target.
		vector<Transform*> generatedPieces(freeAttachments.size());
		for (int i = 0; i < generatedPieces.size(); i++)
		{
			generatedPieces[i] = new Transform();

			// NOTE: Need to find a way to fetch the proper attachment point here.
			Transform* sourceConnector; // = getAttachmentPoint(generatedPieces[i]);
			Transform* otherConnector = popTransformVector(freeAttachments);

			connectPieces(generatedPieces[i], sourceConnector, p_targetPiece, 
							otherConnector);

		}
	}

}

void LevelGenSystem::getFreeAttachments( Transform* p_piece, 
										vector<Transform*>& out_attachments )
{
	// NOTE: This method is subjected to change, and is not yet finished.

	// Here, lookup the attachment points of the piece and add them to the list of
	// attachments.
	int nrOfAttachments = 0;

	// Verify that the attachment is not occupied. This is done by checking the
	// corresponding transform element in the transform hierarchy map.
	TransformNode* node = m_transformHierarchy[p_piece];
	if (node == NULL)
	{
		// If there is no node attached here, it means that the piece never have used its
		// attachment points at all. In that case, it must be initialized.
		node = new TransformNode();
		node->children.resize(nrOfAttachments);
		// NOTE: SHOULD NOT BE NULL
		node->parent = NULL;
		node->transform = p_piece;

		m_transformHierarchy[p_piece] = node;
		
	}
	
	// Check for a unoccupied slot
	for (int i = 0; i < node->children.size(); i++)
	{
		if ( !node->children[i] )
			out_attachments.push_back(node->children[i]);
	}
}

void LevelGenSystem::connectPieces( Transform* p_sourcePiece, Transform* p_sourceConnector,
								   Transform* p_otherPiece, Transform* p_otherConnector )
{
	// NOTE: Currently assuming that all transform info is in world space.

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

Transform* LevelGenSystem::popTransformVector( vector<Transform*>& p_vector )
{
	Transform* transform = NULL;
	if ( !p_vector.empty() )
	{
		transform = p_vector.back();
		p_vector.pop_back();
	}
	return transform;
}

void LevelGenSystem::debugPrintTransformTree()
{
	stringstream ss;
	ss << "Level tree hierarchy:\n";

	debugPrintTransformNode(m_transformHierarchy[m_rootTransform], ss, 0);
	DEBUGPRINT((ss.str().c_str()));
}

void LevelGenSystem::debugPrintTransformNode( TransformNode* p_node, 
											 stringstream& p_stream,
											 int currentDepth)
{
	if (p_node)
	{
		for (int i = 0; i < currentDepth; i++)
			p_stream << '\t';

		p_stream << "-> " << p_node->transform << '\n';

		currentDepth++;
		for (int i = 0; i < p_node->children.size(); i++)
			debugPrintTransformNode(m_transformHierarchy[p_node->children[i]], p_stream, 
									currentDepth);
	}
}
