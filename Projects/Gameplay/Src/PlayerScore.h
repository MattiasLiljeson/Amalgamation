#pragma once

#include <Component.h>

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

class PlayerScore: public Component
{
public:
	PlayerScore();

	~PlayerScore();

	float getScore();
	const float* getScorePointer();

	///-----------------------------------------------------------------------------------
	/// Add a relative score.
	/// \param p_relativeScore
	/// \return void
	///-----------------------------------------------------------------------------------
	void addRelativeScore(float p_relativeScore);

	///-----------------------------------------------------------------------------------
	/// Reset the score to an absolute value. NOTE! This will reset the current score!
	/// \param p_absoluteScore
	/// \return void
	///-----------------------------------------------------------------------------------
	void setAbsoluteScore(float p_absoluteScore);

private:
	float m_score; // The accumulated score of the player
};