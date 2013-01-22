#include "DisplayPlayerScoreSystem.h"

#include <AntTweakBarWrapper.h>
#include "PlayerScore.h"
#include "NetworkSynced.h"
#include <sstream>

DisplayPlayerScoreSystem::DisplayPlayerScoreSystem()
	: EntitySystem( SystemType::DisplayPlayerScoreSystem,
	2, ComponentType::PlayerScore, ComponentType::NetworkSynced )
{
}

DisplayPlayerScoreSystem::~DisplayPlayerScoreSystem()
{
}

void DisplayPlayerScoreSystem::inserted( Entity* p_entity )
{
	PlayerScore* score =
		static_cast<PlayerScore*>(p_entity->getComponent( ComponentType::PlayerScore ));
	NetworkSynced* netSync =
		static_cast<NetworkSynced*>(p_entity->getComponent( ComponentType::NetworkSynced ));
	stringstream ss;
	ss << "score(" << netSync->getNetworkOwner() << ")";
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( AntTweakBarWrapper::OVERALL,
		ss.str().c_str(), TwType::TW_TYPE_INT32, score->getScorePointer(), "" );
}

bool DisplayPlayerScoreSystem::checkProcessing()
{
	return false;
}
