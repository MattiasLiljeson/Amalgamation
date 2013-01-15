#pragma once

#include <vector>

using namespace std;

struct AglMatrix;

// =======================================================================================
//                                ConnectionPointCollection
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Collection of connection point data
///        
/// # HardPointCollection
/// Detailed description.....
/// Created on: 8-1-2013 
///---------------------------------------------------------------------------------------

struct ConnectionPointCollection
{
public:
	vector<AglMatrix> m_collection;

};