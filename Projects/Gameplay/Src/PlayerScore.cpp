#include "PlayerScore.h"

PlayerScore::PlayerScore()
	: Component( ComponentType::PlayerScore )
{
	m_score=0.0f;
}

PlayerScore::~PlayerScore()
{
}

float PlayerScore::getScore()
{
	return m_score;
}

const float* PlayerScore::getScorePointer()
{
	return &m_score;
}


void PlayerScore::addRelativeScore( float p_relativeScore )
{
	m_score+=p_relativeScore;
}

void PlayerScore::setAbsoluteScore( float p_absoluteScore )
{
	m_score = p_absoluteScore;
}
