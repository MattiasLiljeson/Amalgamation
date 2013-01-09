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
	ConnectionPointCollection(bool transformByParent=false) 
	{
		m_localSpace=!transformByParent;
		clear();
	}
	void clear()
	{
		m_meshId = -1;
		m_collection.clear();
	}
	RawTransformData getConnectionPoint(int p_pos)
	{
		return m_collection[p_pos];
	}

	vector<RawTransformData> m_collection;
	int	m_meshId;
	bool isLocalSpace() {return m_localSpace;}
private:
	bool m_localSpace; // not used yet
};