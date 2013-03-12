#pragma once

#include <vector>
#include <string>
#include <utility>
#include <AglMatrix.h>
#include <AglVector3.h>
using namespace std;


struct AmbientRangeSoundCreationData
{
	AglMatrix transform;
	string filename;
	float minRange,maxRange;
	float volume;
	int channels;
};


// =======================================================================================
//                            AmbientRangeSoundDataCollection
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Collection of sound creation data
///        
/// # AmbientRangeSoundDataCollection
/// Detailed description.....
/// Created on: 4-2-2013 
///---------------------------------------------------------------------------------------
struct AmbientRangeSoundDataCollection
{
public:
	vector<AmbientRangeSoundCreationData> m_collection;
};