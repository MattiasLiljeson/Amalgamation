#pragma once

#include <Component.h>
#include "ConnectionPointSet.h"

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

class Connector1to2Module: public Component
{
public:
	Connector1to2Module();
	Connector1to2Module(ConnectionPoint p_target1, ConnectionPoint p_target2);
	~Connector1to2Module();

	ConnectionPoint m_target1;
	ConnectionPoint m_target2;
};