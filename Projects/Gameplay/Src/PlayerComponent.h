#pragma once

#include <Component.h>
#include <string>

using namespace std;

// =======================================================================================
//	PlayerScore
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # PlayerScore
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class PlayerComponent: public Component
{
public:
	PlayerComponent();

	~PlayerComponent();

	float getScore();
	const float* getScorePointer();

	///-----------------------------------------------------------------------------------
	/// Add a relative score.
	/// \param p_relativeScore
	/// \return void
	///-----------------------------------------------------------------------------------
	void addRelativeScore(float p_relativeScore);

	///-----------------------------------------------------------------------------------
	/// Add a relative damage score.
	/// \param p_relativeScore
	/// \return void
	///-----------------------------------------------------------------------------------
	void addRelativeDamageScore(float p_relativeScore);

	float getDealtDamageScore();

	void applyDealtDamageScore();

	///-----------------------------------------------------------------------------------
	/// Reset the score to an absolute value. NOTE! This will reset the current score!
	/// \param p_absoluteScore
	/// \return void
	///-----------------------------------------------------------------------------------
	void setAbsoluteScore(float p_absoluteScore);
public:
	string m_playerName;
	int m_ping;
	int m_playerID;
	int m_networkID;
private:
	float m_score; // The accumulated score of the player
	float m_dealtDamageScore; //Accumulated score of dealing damage to other players
};