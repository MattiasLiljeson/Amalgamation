#pragma once

#include <Component.h>
#include <AglMatrix.h>
#include <vector>

using namespace std;



// =======================================================================================
//	ConnectionPoint
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief A connection point that can have an entity attached.
///        
/// # ConnectionPoint
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------
struct ConnectionPoint
{
	AglMatrix cpTransform;
	int		  cpConnectedEntity;

	ConnectionPoint() {cpConnectedEntity=-1;}

	ConnectionPoint(const AglMatrix& p_transform)
	{
		cpTransform = p_transform;
		cpConnectedEntity = -1;
	}
};

// =======================================================================================
//	ConnectionPointSet
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Set of connection points
///        
/// # ConnectionPointSet
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------
class ConnectionPointSet: public Component
{
public:
	ConnectionPointSet();
	ConnectionPointSet(const vector<ConnectionPoint>& p_connectionPoints);
	ConnectionPointSet(const vector<AglMatrix>& p_connectionPoints);
	~ConnectionPointSet();
	vector<ConnectionPoint> m_connectionPoints;
	// int m_highlighted;

};