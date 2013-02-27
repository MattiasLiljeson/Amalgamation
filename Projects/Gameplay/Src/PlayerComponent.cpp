#include "PlayerComponent.h"

PlayerComponent::PlayerComponent()
	: Component( ComponentType::PlayerComponent )
{
	m_score=0.0f;
	string m_playerName = "";
	m_ping = 0;
	m_playerID = -1;
	m_networkID = -1;
}

PlayerComponent::~PlayerComponent()
{
}

float PlayerComponent::getScore()
{
	return m_score;
}

const float* PlayerComponent::getScorePointer()
{
	return &m_score;
}


void PlayerComponent::addRelativeScore( float p_relativeScore )
{
	m_score+=p_relativeScore;
}

void PlayerComponent::setAbsoluteScore( float p_absoluteScore )
{
	m_score = p_absoluteScore;
}
