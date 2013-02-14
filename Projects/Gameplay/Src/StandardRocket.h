#pragma once

#include <Component.h>

// =======================================================================================
//	MineLayerModule
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Describes a module that deploys mines
///        
/// # PlayerScore
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class StandardRocket: public Component
{
public:
	float m_age;
	int m_ownerId;
	StandardRocket();
	~StandardRocket();
private:

};