#include "LevelGenSystem.h"
#include "Transform.h"
#include <AglVector3.h>
#include <vector>

LevelGenSystem::LevelGenSystem() : EntitySystem(SystemType::LevelGenSystem)
{

}

LevelGenSystem::~LevelGenSystem()
{

}

void LevelGenSystem::initialize()
{
	generateLevelPieces(4);
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

	for (int currentDepth = 0; currentDepth < p_maxDepth; currentDepth++)
	{
		std::vector<Transform*> temps;
		for (int i = 0; i < pieces.size(); i++)
		{
			generatePiecesOnPiece(pieces[i], temps);
			temps.clear();
		}
		pieces = vector<Transform*>(temps);
	}



}

void LevelGenSystem::generatePiecesOnPiece( Transform* p_targetPiece, 
										   vector<Transform*>& out_result )
{

}

void LevelGenSystem::getFreeAttachments( Transform* p_piece, 
										vector<Transform*>& out_attachments )
{
	// Here, lookup the attachment points of the piece and add them to the list of
	// attachments.

	// NOTE: This method is subjected to change.
}
