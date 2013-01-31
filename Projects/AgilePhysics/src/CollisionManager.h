#include "RigidBodyBox.h"
#include "RigidBodyConvexHull.h"
#include "RigidBodySphere.h"
#include "RigidBodyMesh.h"
#include "PhyUtility.h"
#include "GJKSolver.h"

extern float theGlobal;

//----------------------------PRIMARY COLLISION FUNCTIONS---------------------------------

///
/// Checks collision between two rigid bodies. Calls more specific methods
/// \param p_r1 Rigid Body 1
/// \param p_r2 Rigid Body 2
/// \param p_data CollisionData
/// \return True if collision. False otherwise. 
///
bool CheckCollision(RigidBody* p_r1, RigidBody* p_r2, PhyCollisionData* p_data);

///-----------------------------------------------------------------------------------
/// Checks collision between a ray and a general rigid body.
/// \param p_ray The Ray
/// \param p_rigidBody The Rigid Body
/// \param p_collisionData Collision result data
/// \return True if collision. False otherwise
///-----------------------------------------------------------------------------------
bool CheckCollision(PhyRay p_ray, RigidBody* p_rigidBody, 
						RayCollisionData* p_collisionData);

bool CheckCollision(const LineSegment& p_lineSegment, RigidBody* p_rigidBody, float& p_t);

///-----------------------------------------------------------------------------------
/// Check collision between a bounding sphere and a rigid body
/// \param p_sphere The sphere
/// \param p_rigidBody The rigid body
/// \return Returns true on collision. False otherwise.
///-----------------------------------------------------------------------------------
bool CheckCollision(AglBoundingSphere p_sphere, RigidBody* p_rigidBody, AglVector3& p_collisionPoint);

///-----------------------------------------------------------------------------------
/// Checks collision between a sphere and a ray
/// \param p_sphere The Sphere
/// \param p_ray The Ray
/// \return Returns true if there is a collision. False otherwise.
///-----------------------------------------------------------------------------------
bool CheckCollision(AglBoundingSphere p_sphere, PhyRay p_ray);

///-----------------------------------------------------------------------------------
/// Checks collision between a sphere and a ray
/// \param p_sphere The Sphere
/// \param p_ray The Ray
/// \return Returns true if there is a collision. False otherwise.
///-----------------------------------------------------------------------------------
bool CheckCollision(RigidBodySphere* p_sphere, PhyRay p_ray, RayCollisionData* p_collisionData);

///-----------------------------------------------------------------------------------
/// Check collision between a ray and a triangle
/// \param p_ray The Ray
/// \param p_triangle The Triangle
/// \return Returns true if there is a collision. False otherwise.
///-----------------------------------------------------------------------------------
bool CheckCollision(PhyRay p_ray, PhyTriangle p_triangle);

///-----------------------------------------------------------------------------------
/// Checks collision between a ray and an AABB
/// \param p_ray The Ray
/// \param p_min Minimum point of the box
/// \param p_max Maximum point of the box
/// \param p_collisionData Data storing collision information
/// \return Returns true if there is a collision. False otherwise.
///-----------------------------------------------------------------------------------
bool CheckCollision(PhyRay p_ray, AglVector3 p_min, AglVector3 p_max, 
						RayCollisionData* p_collisionData);


///-----------------------------------------------------------------------------------
/// Checks collision between a ray and an OBB
/// \param p_ray The Ray
/// \param p_box The OBB
/// \param p_collisionData Data storing collision information
/// \return Returns true if there is a collision. False otherwise.
///-----------------------------------------------------------------------------------
bool CheckCollision(PhyRay p_ray, RigidBodyBox* p_box, 
						RayCollisionData* p_collisionData);

///-----------------------------------------------------------------------------------
/// Checks collision between a ray and a convex hull
/// \param p_ray The Ray
/// \param p_hull The Convex Hull
/// \param p_collisionData Data storing collision information
/// \return Returns true if there is a collision. False otherwise.
///-----------------------------------------------------------------------------------
bool CheckCollision(PhyRay p_ray, RigidBodyConvexHull* p_hull, RayCollisionData* p_collisionData);

bool CheckCollision(const LineSegment& p_lineSegment, RigidBodySphere* p_sphere, float& p_t);
bool CheckCollision(const LineSegment& p_lineSegment, AglVector3 p_min, AglVector3 p_max, float& p_t);
bool CheckCollision(const LineSegment& p_lineSegment, RigidBodyBox* p_box, float& p_t);


///-----------------------------------------------------------------------------------
/// Checks collision between two spheres
/// \param p_sphere1 First Sphere
/// \param p_sphere2 Second Sphere
/// \return Returns true if there is a collision. False otherwise.
///-----------------------------------------------------------------------------------
bool CheckCollision(AglBoundingSphere p_sphere1, AglBoundingSphere p_sphere2, AglVector3& p_collisionPoint); 

///-----------------------------------------------------------------------------------
/// Checks collision between two spheres
/// \param p_sphere1 First Sphere
/// \param p_sphere2 Second Sphere
/// \return Returns true if there is a collision. False otherwise.
///-----------------------------------------------------------------------------------
bool CheckCollision(RigidBodySphere* p_sphere1, RigidBodySphere* p_sphere2);

///-----------------------------------------------------------------------------------
/// Checks collision between two spheres
/// \param p_sphere1 First Sphere
/// \param p_sphere2 Second Sphere
/// \param p_collisionData Data storing collision information
/// \return Returns true if there is a collision. False otherwise.
///-----------------------------------------------------------------------------------
bool CheckCollision(RigidBodySphere* p_sphere1, RigidBodySphere* p_sphere2, 
						PhyCollisionData* p_collisionData);

///-----------------------------------------------------------------------------------
/// Checks collision between a sphere and an OBB
/// \param p_sphere The Sphere
/// \param p_box The OBB
/// \return Returns true if there is a collision. False otherwise.
///-----------------------------------------------------------------------------------
bool CheckCollision(AglBoundingSphere p_sphere, RigidBodyBox* p_box, AglVector3& p_collisionPoint); 

bool CheckCollision(AglBoundingSphere p_sphere, AglOBB p_box); 

///-----------------------------------------------------------------------------------
/// Checks collision between a sphere and an OBB
/// \param p_sphere The Sphere
/// \param p_box The OBB
/// \param p_collisionData Data storing collision information
/// \return Returns true if there is a collision. False otherwise.
///-----------------------------------------------------------------------------------
bool CheckCollision(RigidBodySphere* p_sphere, RigidBodyBox* p_box, 
						PhyCollisionData* p_collisionData);

///-----------------------------------------------------------------------------------
/// Checks collision between a sphere and an AABB
/// \param p_sphere The Sphere
/// \param p_min Minimum point of the AABB
/// \param p_max Maximum point of the AABB
/// \return Returns true if there is a collision. False otherwise. 
///-----------------------------------------------------------------------------------
bool CheckCollision(AglBoundingSphere p_sphere, AglVector3 p_min, AglVector3 p_max); 

///-----------------------------------------------------------------------------------
/// Checks collision between a sphere and a convex hull
/// \param p_sphere The Sphere
/// \param p_hull The Hull
/// \param p_collisionData Data storing collision information
/// \return Returns true if there is a collision. False otherwise.
///-----------------------------------------------------------------------------------
bool CheckCollision(RigidBodySphere* p_sphere, RigidBodyConvexHull* p_hull, 
						PhyCollisionData* p_collisionData);

///-----------------------------------------------------------------------------------
/// Checks collision between a sphere and a convex hull
/// \param p_sphere The Sphere
/// \param p_hull The Hull
/// \return Returns true if there is a collision. False otherwise.
///-----------------------------------------------------------------------------------
bool CheckCollision(AglBoundingSphere p_sphere, RigidBodyConvexHull* p_hull);

///-----------------------------------------------------------------------------------
/// Checks collision between two OBBs
/// \param p_box1 The first OBB
/// \param p_box2 The second OBB
/// \return Returns true if there is a collision. False otherwise. 
///-----------------------------------------------------------------------------------
bool CheckCollision(RigidBodyBox* p_box1, RigidBodyBox* p_box2);

///-----------------------------------------------------------------------------------
/// Checks collision between two OBBs
/// \param p_box1 The first OBB
/// \param p_box2 The second OBB
/// \param p_collisionData Data storing collision information
/// \return Returns true if there is a collision. False otherwise. 
///-----------------------------------------------------------------------------------
bool CheckCollision(RigidBodyBox* p_box1, RigidBodyBox* p_box2, 
						PhyCollisionData* p_collisionData);

///-----------------------------------------------------------------------------------
/// Checks collision between two convex hulls
/// \param p_hull1 First Hull
/// \param p_hull2 Second Hull
/// \param p_collisionData Data storing collision information
/// \return Returns true if there is a collision. False otherwise. 
///-----------------------------------------------------------------------------------
bool CheckCollision(RigidBodyConvexHull* p_hull1, RigidBodyConvexHull* p_hull2, 
						PhyCollisionData* p_collisionData);

///-----------------------------------------------------------------------------------
/// Checks collision between a box and a convex hull
/// \param p_box The Box
/// \param p_hull The Hull
/// \param p_collisionData Data storing collision information
/// \return Returns true if there is a collision. False otherwise. 
///-----------------------------------------------------------------------------------
bool CheckCollision(RigidBodyBox* p_box, RigidBodyConvexHull* p_hull, 
						PhyCollisionData* p_collisionData);

bool CheckCollision(RigidBodySphere* p_sphere, RigidBodyMesh* p_mesh, 
					PhyCollisionData* p_collisionData);

bool CheckCollision(RigidBodyBox* p_box, RigidBodyMesh* p_mesh, 
					PhyCollisionData* p_collisionData);

bool CheckCollision(RigidBodyConvexHull* p_hull, RigidBodyMesh* p_mesh, 
					PhyCollisionData* p_collisionData);

bool CheckCollision(RigidBodyMesh* p_mesh1, RigidBodyMesh* p_mesh2, 
					PhyCollisionData* p_collisionData);

bool CheckCollision(const AglBoundingSphere& p_sphere, const AglVector3& p_v1, const AglVector3& p_v2, const AglVector3& p_v3,
						EPACollisionData* p_epaData);

bool CheckCollision(const AglVector3& p_t11, const AglVector3& p_t12, const AglVector3& p_t13,
					const AglVector3& p_t21, const AglVector3& p_t22, const AglVector3& p_t23,
					EPACollisionData* p_epaData);

//--------------------------------SUPPORT FUNCTIONS---------------------------------------


///-----------------------------------------------------------------------------------
/// Checks the overlap between two boxes along a given axis
/// \param p_box1 The first box
/// \param p_box2 The second box
/// \param p_axis The axis
/// \return The overlap amount. Zero if non-overlapping.
///-----------------------------------------------------------------------------------
float OverlapAmount(RigidBodyBox* p_box1, RigidBodyBox* p_box2, AglVector3 p_axis);

///-----------------------------------------------------------------------------------
/// Calculates the interval when a box is projected on an axis
/// \param p_box The Box
/// \param p_axis The Axis. Normalization is not imposed.
/// \param[out] p_min The minimum value in the interval
/// \param[out] p_max The maximum value in the interval
///-----------------------------------------------------------------------------------
void  CalculateProjectionInterval(RigidBodyBox* p_box, AglVector3 p_axis, 
									float& p_min, float& p_max);


float OverlapAmount(const vector<AglVector3>& p_points1, const vector<AglVector3>& p_points2, const AglVector3& p_axis);

void  CalculateProjectionInterval(const vector<AglVector3>& p_points, const AglVector3& p_axis, 
								  float& p_min, float& p_max);

float OverlapAmount(const AglVector3* p_points1, const AglVector3* p_points2, const AglVector3& p_axis);

void  CalculateProjectionInterval(const AglVector3* p_points, const AglVector3& p_axis, 
								  float& p_min, float& p_max);


///-----------------------------------------------------------------------------------
/// Given a box, a collision axis and a penetration amount find the hit points on the box.
/// \param p_box The box
/// \param p_axis The collision axis
/// \param p_penetration The penetration amount
/// \return The list of hit points. Size should be 1, 2 or 4.
///-----------------------------------------------------------------------------------
vector<AglVector3> GetHitPoints(RigidBodyBox* p_box, AglVector3 p_axis, float p_penetration);


///-----------------------------------------------------------------------------------
/// Given a list of points, a collision axis and a penetration amount find the hit points among the points.
/// \param p_points The points
/// \param p_axis The collision axis
/// \param p_penetration The penetration amount
/// \return The list of hit points. Currently limited to 1, 2 or 3. 
///-----------------------------------------------------------------------------------
vector<AglVector3> GetHitPoints(vector<AglVector3> p_points, AglVector3 p_axis, 
								float p_penetration);

///-----------------------------------------------------------------------------------
/// Finds the overlap, a polygon shape, between two quads projected on a plane
/// \param p_quad1 First quad. List size assumed to be 4.
/// \param p_quad2 Second quad. List size assumed to be 4.
/// \param p_axis The axis used for the plane normal
/// \param p_penetration Penetration used for plane offset
/// \return List of points creating the overlapping shape. Size should be 3 or 4
///-----------------------------------------------------------------------------------
vector<AglVector3> FindOverlapQuads(vector<AglVector3> p_quad1, vector<AglVector3> p_quad2, 
									AglVector3 p_axis, float p_penetration);

///-----------------------------------------------------------------------------------
/// Finds the overlap, a polygon shape, between two triangles projected on a plane
/// \param p_triangle1 First triangle. List size assumed to be 3.
/// \param p_triangle2 Second triangle. List size assumed to be 3.
/// \param p_axis The axis used for the plane normal
/// \param p_penetration Penetration used for plane offset
/// \return List of points creating the overlapping shape. Size should be 3 or 4
///-----------------------------------------------------------------------------------
vector<AglVector3> FindOverlapTriangles(vector<AglVector3> p_triangle1, 
										vector<AglVector3> p_triangle2, 
											AglVector3 p_axis, float p_penetration);

///-----------------------------------------------------------------------------------
/// Finds the overlap, a polygon shape, between a quad and a triangle projected on a plane
/// \param p_quad Quad. List size assumed to be 4.
/// \param p_triangle Triangle. List size assumed to be 3.
/// \param p_axis The axis used for the plane normal
/// \param p_penetration Penetration used for plane offset
/// \return List of points creating the overlapping shape. Size should be 3 or 4
///-----------------------------------------------------------------------------------
vector<AglVector3> FindOverlapQuadTriangle(vector<AglVector3> p_quad, 
											vector<AglVector3> p_triangle, 
												AglVector3 p_axis, 
													float p_penetration);

///-----------------------------------------------------------------------------------
/// Finds the overlap, an edge, between a quad and an edge projected on a plane
/// \param p_edgeStart Start point of the edge
/// \param p_edgeEnd End point of the edge
/// \param p_quad Quad. List size assumed to be 4.
/// \param p_axis The axis used for the plane normal
/// \param p_penetration Penetration used for plane offset
/// \return List of points creating the overlapping edge. Size should be 2
///-----------------------------------------------------------------------------------
vector<AglVector3> FindOverlapEdgeQuad(AglVector3 p_edgeStart, AglVector3 p_edgeEnd, 
										vector<AglVector3> p_quad, AglVector3 p_axis, 
											float p_penetration);

///-----------------------------------------------------------------------------------
/// Finds the overlap, an edge, between a triangle and an edge projected on a plane
/// \param p_edgeStart Start point of the edge
/// \param p_edgeEnd End point of the edge
/// \param p_triangle Triangle. List size assumed to be 3.
/// \param p_axis The axis used for the plane normal
/// \param p_penetration Penetration used for plane offset
/// \return List of points creating the overlapping edge. Size should be 2
///-----------------------------------------------------------------------------------
vector<AglVector3> FindOverlapEdgeTriangle(AglVector3 p_edgeStart, AglVector3 p_edgeEnd, 
											vector<AglVector3> p_triangle, AglVector3 p_axis, 
												float p_penetration);

///-----------------------------------------------------------------------------------
/// Sorts a list of points representing a quad. The sorted points will be given in 
/// clockwise or counterclockwise order.
/// \param p_quad The quad to sort.
/// \return The sorted list of points
///-----------------------------------------------------------------------------------
vector<AglVector3> SortQuad(vector<AglVector3> p_quad);


///-----------------------------------------------------------------------------------
/// Finds the distance between two edges
/// \param p_e1Start Start of edge1.
/// \param p_e1End End of edge1.
/// \param p_e2Start Start of edge2.
/// \param p_e2End End of edge2.
/// \param[out] p_s Reference to the closest point on edge1 
/// \param[out] p_t Reference to the closest point on edge2
/// \return The distance
///-----------------------------------------------------------------------------------
float EdgeEdgeDistance(AglVector3 p_e1Start, AglVector3 p_e1End, AglVector3 p_e2Start, 
						AglVector3 p_e2End, float& p_s, float& p_t);
