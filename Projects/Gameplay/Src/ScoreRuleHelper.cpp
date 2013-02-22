#include "ScoreRuleHelper.h"


int ScoreRuleHelper::myscore_AttachModule( int p_moduleValue, bool p_moduleIsUnused )
{
	if (p_moduleIsUnused)
		return p_moduleValue;
	else
		return p_moduleValue/2;
}

int ScoreRuleHelper::myscore_HittingOpponent( int p_opponentHitModuleValue )
{
	return p_opponentHitModuleValue/2;
}

int ScoreRuleHelper::myscore_LoseModuleOnDetach( int p_moduleValue )
{
	return -p_moduleValue/2;
}

int ScoreRuleHelper::myscore_LoseModuleOnEnemyHit( int p_myHitModuleValue )
{
	return -p_myHitModuleValue/2;
}
