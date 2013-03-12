#pragma once

// =======================================================================================
//                                    ScoreRuleHelper
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	This class is the embodiment of the score rules. A score to be set to a player
/// can be calculated by this class based on scenario and module values.
///        
/// # ScoreRuleHelper
/// Detailed description.....
/// Created on: 22-2-2013 
///---------------------------------------------------------------------------------------

class ScoreRuleHelper
{
public:
	ScoreRuleHelper();
	virtual ~ScoreRuleHelper();

	///-----------------------------------------------------------------------------------
	/// The score that the player gets by attaching a module.
	/// \param p_moduleValue
	/// \return int
	///-----------------------------------------------------------------------------------
	static float scoreFromAttachModule(float p_moduleValue, bool p_moduleIsUnused);


	///-----------------------------------------------------------------------------------
	/// The score that the player gets by hitting an enemy player with a weapon.
	/// \param p_opponentHitModuleValue
	/// \return int
	///-----------------------------------------------------------------------------------
	static float scoreFromHittingOpponent(float p_opponentHitModuleValue);


	///-----------------------------------------------------------------------------------
	/// Score that the player loses by detaching a module in edit mode
	/// \param p_moduleValue
	/// \return int
	///-----------------------------------------------------------------------------------
	static float scoreFromLoseModuleOnDetach(float p_moduleValue);


	///-----------------------------------------------------------------------------------
	/// The score that the player loses by losing a module when hit by an enemy weapon
	/// \param p_myHitModuleValue
	/// \return int
	///-----------------------------------------------------------------------------------
	static float scoreFromLoseModuleOnEnemyHit(float p_myHitModuleValue);
};