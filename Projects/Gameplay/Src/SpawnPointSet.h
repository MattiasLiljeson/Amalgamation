#pragma once

#include <Component.h>
#include <AglMatrix.h>
#include <vector>

using namespace std;


// =======================================================================================
//                                      SpawnPoint
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Struct containing a spawn transform and an action to take 
/// by the system using the spawn point
///        
/// # SpawnPoint
/// Detailed description.....
/// Created on: 4-2-2013 
///---------------------------------------------------------------------------------------
struct SpawnPoint
{
	AglMatrix spTransform;
	string	  spAction;

	SpawnPoint() {spAction="";}

	SpawnPoint(const AglMatrix& p_transform, 
		const string& p_action)
	{
		spTransform = p_transform;
		spAction = p_action;
	}
};

// =======================================================================================
//                                      SpawnPointSet
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # SpawnPointSet
/// Detailed description.....
/// Created on: 4-2-2013 
///---------------------------------------------------------------------------------------
class SpawnPointSet: public Component
{
public:
	SpawnPointSet();
	SpawnPointSet(const vector<SpawnPoint>& p_spawnPoints);
	~SpawnPointSet();
	vector<SpawnPoint> m_spawnPoints;
};