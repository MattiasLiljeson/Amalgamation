#pragma once

#include "RawTransformData.h"
#include <vector>

using namespace std;

struct RawTransformData;

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
	ConnectionPointCollection(bool transformByParent=false) {m_localSpace=!transformByParent;}
	vector<RawTransformData> m_collection;
	bool isLocalSpace() {return m_localSpace;}
private:
	bool m_localSpace; // not used yet
};