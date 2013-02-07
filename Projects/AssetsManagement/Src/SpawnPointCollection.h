#pragma once

#include <vector>
#include <string>
#include <utility>
using namespace std;

struct AglMatrix;

// =======================================================================================
//                                SpawnPointCollection
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Collection of spawn point data
///        
/// # SpawnPointCollection
/// Detailed description.....
/// Created on: 4-2-2013 
///---------------------------------------------------------------------------------------

struct SpawnPointCollection
{
public:
	vector<pair<string,AglMatrix>> m_collection;
};