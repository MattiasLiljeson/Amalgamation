#include "PlayerScore.h"

PlayerScore::PlayerScore()
{
	m_baseScore = 0;
	m_moduleScore = 0;
}

PlayerScore::~PlayerScore()
{
}

float PlayerScore::getTotalScore()
{
	m_totalScore = m_moduleScore+m_baseScore;
	return m_totalScore;
}
float* PlayerScore::getTotalScorePointer()
{
	return &m_totalScore;
}
void PlayerScore::setModuleScore(float p_score)
{
	m_moduleScore = p_score;
}