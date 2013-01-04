#include "PlayerScore.h"

PlayerScore::PlayerScore()
{
	m_score = 0;
}

PlayerScore::PlayerScore( int p_score )
{
	m_score = p_score;
}

PlayerScore::~PlayerScore()
{
}

int PlayerScore::getScore() const
{
	return m_score;
}

void PlayerScore::setScore( int p_score )
{
	m_score = p_score;
}

int* PlayerScore::getScorePointer()
{
	return &m_score;
}
