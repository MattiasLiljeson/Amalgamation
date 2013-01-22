#include "DisplayPlayerScoreSystem.h"

#include <AntTweakBarWrapper.h>
#include "PlayerScore.h"
#include "NetworkSynced.h"
#include <sstream>

DisplayPlayerScoreSystem::DisplayPlayerScoreSystem()
	: EntitySystem( SystemType::DisplayPlayerScoreSystem,
	1, ComponentType::PlayerScore )
{
}

DisplayPlayerScoreSystem::~DisplayPlayerScoreSystem()
{
}

void DisplayPlayerScoreSystem::processEntities( const vector<Entity*>& p_entities )
{
}

void DisplayPlayerScoreSystem::initialize()
{

}

void DisplayPlayerScoreSystem::added( Entity* p_entity )
{
	PlayerScore* score =
		static_cast<PlayerScore*>(p_entity->getComponent( ComponentType::PlayerScore ));
	NetworkSynced* netSync =
		static_cast<NetworkSynced*>(p_entity->getComponent( ComponentType::NetworkSynced ));

	if( score != NULL && netSync != NULL )
	{
		stringstream ss;
		ss << "score(" << netSync->getNetworkOwner() << ")";

		AntTweakBarWrapper::getInstance()->addReadOnlyVariable( AntTweakBarWrapper::OVERALL,
			ss.str().c_str(), TwType::TW_TYPE_INT32, score->getScorePointer(), "" );
	}
}
