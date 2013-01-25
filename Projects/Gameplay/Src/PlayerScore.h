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

	float getTotalScore();
	float* getTotalScorePointer();
	void setModuleScore(float p_score);
	void addBaseScore(float p_score);

private:
	float m_moduleScore; ///< Variable score based on modules
	float m_baseScore; ///< Base Score. Cannot be lost. Only gained 
	float m_totalScore; ///< Total Score

};