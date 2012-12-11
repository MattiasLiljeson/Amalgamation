// =======================================================================================
//                                         GJKEPASOLVER
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief GJK and EPA solver that handles collision queries
///
/// 
/// The GJK solver checks for collision between to convex solids.
/// The EPA solver finds the collision normal and penetration
/// amount between colliding solids.
/// Edited By: Anton Andersson, 2012-11-26
///---------------------------------------------------------------------------------------


#ifndef GJKSOLVER_H
#define GJKSOLVER_H

#include "RigidBody.h"

static int GJKSIZE; ///< Current Size of the GJK simplex
static AglVector3 GJKLIST[4]; ///< GJK simplex. Specifies point, line, triangle or tetra

// =======================================================================================
//                                         EPAEDGE
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Line edge in an EPA shape
/// Edited By: Anton Andersson, 2012-11-26
///---------------------------------------------------------------------------------------
struct EPAEdge
{ 
	AglVector3 A; ///< First point on edge
	AglVector3 B; ///< Second point on edge

	///-----------------------------------------------------------------------------------
	/// Constructor
	/// \param p_a First point
	/// \param p_b Second point
	///-----------------------------------------------------------------------------------
	EPAEdge(AglVector3 p_a, AglVector3 p_b)
	{
		A = p_a;
		B = p_b;
	}

	///-----------------------------------------------------------------------------------
	/// Checks if two edges are equal
	/// \param p_other The edge to check against
	/// \return Boolean indicating if they are equal
	///-----------------------------------------------------------------------------------
	bool Equals(EPAEdge p_other)
	{
		if (p_other.A == A && p_other.B == B)
			return true;
		if (p_other.A == B && p_other.B == A)
			return true;
		return false;
	}
};

// =======================================================================================
//                                         EPATriangle
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Triangle used in an EPA simplex. Triangles always point away from origo.
/// Edited By: Anton Andersson, 2012-11-26
///---------------------------------------------------------------------------------------
struct EPATriangle
{
	///-----------------------------------------------------------------------------------
	/// Constructor. Ensures that the triangle is pointing away from origo
	/// \param Point One
	/// \param Point Two
	/// \param Point Three
	///-----------------------------------------------------------------------------------
	EPATriangle(AglVector3 pA, AglVector3 pB, AglVector3 pC)
	{
		A = pA;
		B = pB;
		C = pC;

		N = AglVector3::crossProduct(B-A, C-A);
		AglVector3::normalize(N);
		if (AglVector3::dotProduct(A, N) < 0)
			N = -N;
	}
	AglVector3 A; ///< Point 1
	AglVector3 B; ///< Point 2
	AglVector3 C; ///< Point 3
	AglVector3 N; ///< Normal
};

// =======================================================================================
//                                         EPACollisionData
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Collision information used by collision solver
/// Edited By: Anton Andersson, 2012-11-26
///---------------------------------------------------------------------------------------
struct EPACollisionData
{
	float	Depth; ///< Depth of the collision
	AglVector3 Normal; ///< Normal of the collision
};

//GJK-------------------------------------------------------------------------------------


///-----------------------------------------------------------------------------------
/// Checks collision between two convex point sets
/// \param p_a Point set 1
/// \param p_b Point set 2
/// \param[out] p_data CollisionData to be used by solver
/// \return Returns true on intersection. False otherwise.
///-----------------------------------------------------------------------------------
bool		gjkCheckCollision(const vector<AglVector3>& p_a, const vector<AglVector3>& p_b, EPACollisionData* p_data);

///-----------------------------------------------------------------------------------
/// Checks collision between a convex point set and a sphere
/// \param p_a Point set 1
/// \param p_sphere Sphere
/// \param[out] p_data CollisionData to be used by solver
/// \return Returns true on intersection. False otherwise.
///-----------------------------------------------------------------------------------
bool		gjkCheckCollision(const vector<AglVector3>& p_a, const AglBoundingSphere& p_sphere, EPACollisionData* p_data);

///---------------------------------------------------------------------------------------
/// Returns a support point from two point sets and a direction. Study GJK for more info.
/// \param p_direction Direction
/// \param p_a First Point Set
/// \param p_b Second Point Set
/// \return Support Point
///---------------------------------------------------------------------------------------
AglVector3		gjkSupport(AglVector3 p_direction, const vector<AglVector3>& p_a, const vector<AglVector3>& p_b);

///---------------------------------------------------------------------------------------
/// Returns a support point from a point set, a sphere and a direction. Study GJK for more info.
/// \param p_direction Direction
/// \param p_a Point Set
/// \param p_sphere The Sphere
/// \return Support Point
///---------------------------------------------------------------------------------------
AglVector3		gjkSupport(AglVector3 p_direction, const vector<AglVector3>& p_a, const AglBoundingSphere& p_sphere);

///-----------------------------------------------------------------------------------
/// Processes the simplex defined by the GJKLIST using p_direction as direction.
/// \param[out] p_direction The direction
/// \return Returns true on collision. False otherwise
///-----------------------------------------------------------------------------------
bool		gjkProcessSimplex(AglVector3& p_direction);

///-----------------------------------------------------------------------------------
/// Processes the line defined by the GJKLIST using p_direction as direction.
/// \param[out] p_direction The direction
/// \return Returns true on collision. False otherwise
///-----------------------------------------------------------------------------------
bool		gjkProcessSimplexLine(AglVector3& p_direction);

///-----------------------------------------------------------------------------------
/// Processes the triangle defined by the GJKLIST using p_direction as direction.
/// \param[out] p_direction The direction
/// \return Returns true on collision. False otherwise
///-----------------------------------------------------------------------------------
bool		gjkProcessSimplexTriangle(AglVector3& p_direction);

///-----------------------------------------------------------------------------------
/// Processes the tetrahedron defined by the GJKLIST using p_direction as direction.
/// \param[out] p_direction The direction
/// \return Returns true on collision. False otherwise
///-----------------------------------------------------------------------------------
bool		gjkProcessSimplexTetra(AglVector3& p_direction);


///-----------------------------------------------------------------------------------
/// Redefine the GJKLIST as a point. Change search direction appropriately
/// \param p_value The point value
/// \param[out] p_direction The direction
///-----------------------------------------------------------------------------------
void		gjkSetPoint(AglVector3 p_value, AglVector3& p_direction);
///-----------------------------------------------------------------------------------
/// Redefine the GJKLIST as a linesegment. Change search direction appropriately
/// \param p_a Start of line segment
/// \param p_b End of line segment
/// \param[out] p_direction The direction
///-----------------------------------------------------------------------------------
void		gjkSetLine(AglVector3 p_a, AglVector3 p_b, AglVector3& p_direction);

///-----------------------------------------------------------------------------------
/// Redefine the GJKLIST as a triangle. Change search direction appropriately
/// \param p_a Point 1
/// \param p_b Point 2
/// \param p_c Point 3
/// \param[out] p_direction The direction
///-----------------------------------------------------------------------------------
void		gjkSetTriangle(AglVector3 p_a, AglVector3 p_b, AglVector3 p_c, AglVector3& p_direction);

//EPA-------------------------------------------------------------------------------------


///-----------------------------------------------------------------------------------
/// Process a simplex using EPA to find the collision normal and penetration depth.
/// \param p_simplex Simplex to process. 
/// \param p_a Point Set 1
/// \param p_b Point Set 2
/// \param[out] p_data Collision information used by collision solver.
///-----------------------------------------------------------------------------------
void					epaProcessCollision(vector<AglVector3>& p_simplex, const vector<AglVector3>& p_a, const vector<AglVector3>& p_b, EPACollisionData* p_data);

///-----------------------------------------------------------------------------------
/// Process a simplex using EPA to find the collision normal and penetration depth.
/// \param p_simplex Simplex to process. 
/// \param p_a Point Set 1
/// \param p_sphere Sphere
/// \param[out] p_data Collision information used by collision solver.
///-----------------------------------------------------------------------------------
void					epaProcessCollision(vector<AglVector3>& p_simplex, const vector<AglVector3>& p_a, const AglBoundingSphere& p_sphere, EPACollisionData* p_data);

///-----------------------------------------------------------------------------------
/// Turn a simplex into a hull of triangles
/// \param p_points Source points of the simplex 
/// \return The new simplex made out of triangles
///-----------------------------------------------------------------------------------
vector<EPATriangle>		epaConstructTriangleSimplex(vector<AglVector3>& p_points);

///-----------------------------------------------------------------------------------
/// Find the triangle closest to origo from a simplex consisting of triangles.
/// \param p_simplex Simplex of triangles
/// \param[out] p_minDist Minimum Distance 
/// \return Index to the triangle
///-----------------------------------------------------------------------------------
int						epaFindClosestTriangle(const vector<EPATriangle>& p_simplex, float& p_minDist);

///-----------------------------------------------------------------------------------
/// Insert a point into a triangle simplex. This method practically expands the hull.
/// \param p_simplex Simplex to expand
/// \param p_point Point to add
///-----------------------------------------------------------------------------------
void					epaInsertPoint(vector<EPATriangle>& p_simplex, AglVector3 p_point);

///-----------------------------------------------------------------------------------
/// Inserts an edge into a list of edges. If the edge is present, both edges are removed
/// \param p_edges List of edges
/// \param p_new Edge to add
///-----------------------------------------------------------------------------------
void					epaInsertUnique(vector<EPAEdge>& p_edges, EPAEdge p_new);





//Removed
//void EPATetraFromLine(vector<AglVector3>& pSimplex, const vector<AglVector3>& pA, const vector<AglVector3>& pB);
//void EPATetraFromTriangle(vector<AglVector3>& pSimplex, const vector<AglVector3>& pA, const vector<AglVector3>& pB);
//bool EPAOriginInTetra(AglVector3 pA, AglVector3 pB, AglVector3 pC, AglVector3 pD);
//bool		gjkProcessSimplexTetraOld(vector<AglVector3>& pPointList, AglVector3& pDirection);
//bool		gjkInside(AglVector3 pA, AglVector3 pB, AglVector3 pC, AglVector3 pCenter);



#endif