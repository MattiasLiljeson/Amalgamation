#include "ScoreRuleHelper.h"


float ScoreRuleHelper::scoreFromAttachModule( float p_moduleValue, bool p_moduleIsUnused )
{
	if (p_moduleIsUnused)
		return p_moduleValue;
	else
		return p_moduleValue/2;
}

float ScoreRuleHelper::scoreFromHittingOpponent( float p_opponentHitModuleValue )
{
	return p_opponentHitModuleValue/2;
}

float ScoreRuleHelper::scoreFromLoseModuleOnDetach( float p_moduleValue )
{
	return -p_moduleValue/2;
}

float ScoreRuleHelper::scoreFromLoseModuleOnEnemyHit( float p_myHitModuleValue )
{
	return -p_myHitModuleValue/2;
}
float ScoreRuleHelper::scoreFromDamagingOpponent(float p_damage)
{
	return p_damage / 4.0f;
}