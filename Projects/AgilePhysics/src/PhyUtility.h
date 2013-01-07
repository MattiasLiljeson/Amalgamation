#ifndef PHYUTILITY_H
#define PHYUTILITY_H

#include <AglVector3.h>
#include <AglMatrix.h>
#include <AglVector4.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <ctime>

using namespace std;

const float ONE_METER = 1.0f;
const float ONE_KILOGRAM = 1.0f;
const float ONE_SECOND = 1.0f;

class RigidBody;

// =================================================================================================
//                                         PhyPlane
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Represents a plane using a normal and and an offset. x*n = d
///
/// 
/// # PhyPlane
/// Edited By: Anton Andersson, 2012-11-22
///-------------------------------------------------------------------------------------------------
struct PhyPlane
{
	float D; ///< Offset from origo
	AglVector3 N; ///< Normal

	///
	/// Constructor
	/// \param p_n Normal
	/// \param p_d Offset from origo
	///
	PhyPlane(AglVector3 p_n, float p_d)
	{
		N = p_n;
		D = p_d;
	}
};

// =================================================================================================
//                                         PhyRay
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Represents a ray using an origin and a direction. Normalization is not imposed.
///
/// 
/// # PhyRay
/// Edited By: Anton Andersson, 2012-11-22
///-------------------------------------------------------------------------------------------------
struct PhyRay
{
	AglVector3 o;
	AglVector3 d;
};

// =================================================================================================
//                                         PhyCollisionData
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Represents a collision between two rigid bodies using contact information.
///
/// 
/// # PhyCollisionData
/// Edited By: Anton Andersson, 2012-11-22
///-------------------------------------------------------------------------------------------------
struct PhyCollisionData
{
	RigidBody* Body1;
	RigidBody* Body2;
	vector<pair<AglVector3,AglVector3>> Contacts;
};

// =================================================================================================
//                                         PhyTriangle
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Represents a triangle in three dimensions.
///
/// 
/// # PhyTriangle
/// Edited By: Anton Andersson, 2012-11-22
///-------------------------------------------------------------------------------------------------
struct PhyTriangle
{
	AglVector3 p1; ///< Point 1
	AglVector3 p2; ///< Point 2
	AglVector3 p3; ///< Point 3
};

// =================================================================================================
//                                         PhyRayCollisionData
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Data returned by ray collision queries. Incomplete!
///
/// 
/// # PhyRayCollisionData
/// Edited By: Anton Andersson, 2012-11-22
///-------------------------------------------------------------------------------------------------
struct RayCollisionData
{
	float t; ///< Time of the collision. May vary depending on ray direction normalization.
};

// =================================================================================================
//                                         PhyBox
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Data representing a box
///
/// 
/// # PhyBox
/// Edited By: Anton Andersson, 2012-11-22
///-------------------------------------------------------------------------------------------------
struct PhyBox
{
	AglMatrix world; ///< World transform of the box
	AglVector3 size; ///< Size of the box
};

AglVector4 AglVec4MatrixMult(AglVector4& vec, AglMatrix mat);
AglVector3	AglVec3Transform(AglVector3& vec, AglMatrix mat);
AglVector3 AglVec3TransformNormal(AglVector3& vec, AglMatrix mat);

float AglDet(AglVector3 pV1, AglVector3 pV2, AglVector3 pV3);
float AglRandomFloat();
float AglRandomFloat(float minValue, float maxValue);

#endif