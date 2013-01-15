#pragma once

#include <Component.h>
#include <AglMatrix.h>
#include <vector>

using namespace std;


// =======================================================================================
//	ConnectionPointSet
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Describes a module that boosts the ship speed
///        
/// # PlayerScore
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

struct ConnectionPoint
{
	AglMatrix cpTransform;
	int		  cpConnectedEntity;

	ConnectionPoint(AglMatrix p_transform)
	{
		cpTransform = p_transform;
		cpConnectedEntity = -1;
	}
};

class ConnectionPointSet: public Component
{
public:
	ConnectionPointSet();
	~ConnectionPointSet();
	vector<ConnectionPoint> m_connectionPoints;
	int m_highlighted;

};