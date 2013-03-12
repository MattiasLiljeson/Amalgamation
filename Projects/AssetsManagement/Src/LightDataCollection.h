#pragma once

#include <vector>
#include <string>
#include <utility>
#include <AglMatrix.h>
#include <AglVector3.h>
using namespace std;


struct LightCreationData
{
	enum Type
	{
		POINT,
		DIR,
		SPOT
	};
	Type type;
	AglMatrix transform;
	AglVector3 diffuse;
	AglVector3 specular;
	AglVector3 ambient;
	float gloss;
	float range, power;
	AglVector3 attenuation;
};


// =======================================================================================
//                                LightDataCollection
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Collection of light creation data
///        
/// # LightPointCollection
/// Detailed description.....
/// Created on: 4-2-2013 
///---------------------------------------------------------------------------------------
struct LightDataCollection
{
public:
	vector<LightCreationData> m_collection;
};