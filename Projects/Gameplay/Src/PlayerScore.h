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
	PlayerScore( int p_score );

	~PlayerScore();

	int getScore() const;
	void setScore( int p_score );

private:
	int m_score;

};