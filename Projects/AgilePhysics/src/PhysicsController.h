#ifndef PHYSICSCONTROLLER_H
#define PHYSICSCONTROLLER_H

#include "ImpulseSolver.h"
#include "CompoundBody.h"
#include "RigidBodyMesh.h"
#include <AglInteriorSphereGrid.h>
#include <AglLooseBspTree.h>
#include <typeindex>
#include "Octree.h"

struct LineCollisionData
{
	int lineID;
	int bodyID;
	float t;
};
typedef pair<unsigned int, unsigned int> UintPair;

// =================================================================================================
//                                         PhysicsController
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Contains control and update logic for the physics system
///
/// 
/// Edited By: Anton Andersson, 2012-11-26
///-------------------------------------------------------------------------------------------------
class PhysicsController
{
private:
	const unsigned int COLLISION_REPETITIONS; ///< Number of loops for the iterative collision solver
	vector<Body*> mBodies; ///< List of all bodies
	vector<pair<RigidBody*, unsigned int>> mRigidBodies; ///< List of rigid bodies
	vector<CompoundBody*> mCompoundBodies; ///<List of compound bodies

	vector<LineSegment> mLineSegments; //< List of line segments

	vector<ConvexHullShape*>	mConvexHullShapes; ///< Hull Shapes

	vector<UintPair> mCollisions;

	vector<LineCollisionData> mLineSegmentCollisions; ///< Index to line segment first then body

	Octree* mStaticBodies;

	float mTimeAccum;

public:

	///-----------------------------------------------------------------------------------
	/// Constructor
	/// \param p_device Dx11 Device
	/// \param p_deviceContext Dx11 DeviceContext
	///-----------------------------------------------------------------------------------
	PhysicsController();

	///-----------------------------------------------------------------------------------
	/// Destructor
	///-----------------------------------------------------------------------------------
	virtual ~PhysicsController();

	///-----------------------------------------------------------------------------------
	/// Adds a sphere to the set of rigid bodies
	/// \param p_position Position of the sphere
	/// \param p_radius Radius of the sphere
	/// \param p_userControlled Bool indicating whether or not the sphere is user controlled
	/// \return Returns the sphere that was added.
	///-----------------------------------------------------------------------------------
	int AddSphere(AglVector3 p_position, float p_radius, bool p_userControlled = false, CompoundBody* pParent = NULL);

	int AddSphere(AglMatrix p_world, float p_radius, float p_mass, AglVector3 p_velocity, AglVector3 p_angularVelocity, bool p_static = false, CompoundBody* pParent = NULL, bool pImpulseEnabled = NULL,
		bool pCollisionEnabled = true);

	///-----------------------------------------------------------------------------------
	/// Adds a box to the set of rigid bodies 
	/// \param p_position Position of the box
	/// \param p_size Size of the box 
	/// \param p_mass Mass of the box
	/// \param p_velocity Initial velocity of the box
	/// \param p_angularVelocity Initial angular velocity of the box
	/// \param p_static Bool indicating whether or not the box is immovable
	/// \return The box
	///-----------------------------------------------------------------------------------
	int AddBox(AglVector3 p_position, AglVector3 p_size, float p_mass, AglVector3 p_velocity, AglVector3 p_angularVelocity, bool p_static = false, CompoundBody* pParent = NULL,
					bool pImpulseEnabled = NULL, bool pCollisionEnabled = true);

	int AddBox(AglOBB p_shape, float p_mass, AglVector3 p_velocity, AglVector3 p_angularVelocity, bool p_static = false, CompoundBody* pParent = NULL, bool pImpulseEnabled = NULL,
				bool pCollisionEnabled = true);

	int AddBox(AglMatrix p_world, AglVector3 p_size, float p_mass, AglVector3 p_velocity, AglVector3 p_angularVelocity, bool p_static = false, CompoundBody* pParent = NULL, bool pImpulseEnabled = NULL,
		bool pCollisionEnabled = true);

	///-----------------------------------------------------------------------------------
	/// Adds a convex hull to the set of rigid bodies 
	/// \param p_position Position of the hull
	/// \param p_size Size of the hull 
	/// \param p_mass Mass of the hull
	/// \param p_velocity Initial velocity of the hull
	/// \param p_angularVelocity Initial angular velocity of the hull
	/// \param p_static Bool indicating whether or not the hull is immovable
	/// \return The hull
	///-----------------------------------------------------------------------------------
	int AddConvexHull(AglVector3 p_position, float p_size, float p_mass, AglVector3 p_velocity, AglVector3 p_angularVelocity, bool p_static = false, CompoundBody* pParent = NULL);

	int AddCompoundBody(AglVector3 p_position);

	int AddMeshBody(AglVector3 pPosition, AglOBB pOBB, AglBoundingSphere pBoundingSphere, AglLooseBspTree* pBSPTree = NULL,
		AglInteriorSphereGrid* pSphereGrid = NULL);

	int AddMeshBody(AglMatrix pWorld, AglOBB pOBB, AglBoundingSphere pBoundingSphere, AglVector3 pSize, AglLooseBspTree* pBSPTree = NULL);

	int AddLineSegment(AglVector3 p_p1, AglVector3 p_p2);
	void SetLineSegment(AglVector3 p_p1, AglVector3 p_p2, int p_index);
	int AddRay(AglVector3 p_o, AglVector3 p_dir, float maxLength = 1000000000);
	void SetRay(AglVector3 p_o, AglVector3 p_dir, int p_index, float maxLength = 1000000000);
	void GetRay(unsigned int p_index, AglVector3& p_o, AglVector3& p_d); 

	void DetachBodyFromCompound(RigidBody* p_body, bool p_impulseEnabled = true, CompoundBody* p_compound = NULL);
	
	void AttachBodyToCompound(CompoundBody* p_compound, RigidBody* p_body, AglMatrix p_localTransform);

	///-----------------------------------------------------------------------------------
	/// Clears all rigid bodies
	///-----------------------------------------------------------------------------------
	void Clear();

	///-----------------------------------------------------------------------------------
	/// Updates the entire physics system
	/// \param p_elapsedTime Timestep
	///-----------------------------------------------------------------------------------
	void Update(float p_elapsedTime);

	///-----------------------------------------------------------------------------------
	/// Checks a set of rays against the set of rigid bodies and returns the closest collision
	/// \param p_rays The rays
	/// \param p_ignore A rigid body to ignore
	/// \return Time of the closest collision
	///-----------------------------------------------------------------------------------
	float RaysVsObjects(vector<PhyRay> p_rays, RigidBody* p_ignore, AglBoundingSphere p_sphere);

	int FindClosestCollision(AglVector3 p_p1, AglVector3 p_p2, int p_avoid = -1);

	vector<RigidBody*> getBodies()
	{ 
		vector<RigidBody*> bodies;
		for (unsigned int i = 0; i < mRigidBodies.size(); i++)
			bodies.push_back(mRigidBodies[i].first);
		return bodies; 
	}

	Body* getBody(int pIndex){ return mBodies[pIndex]; }

	void ApplyExternalImpulse(int p_id, AglVector3 p_impulse, AglVector3 p_angularImpulse);
	void ApplyExternalImpulse(AglVector3 p_position, float p_radius, float p_magnitude);

	bool IsColliding(unsigned int p_b1, unsigned int p_b2);
	vector<unsigned int> CollidesWith(unsigned int p_b);

	vector<unsigned int> LineCollidesWith(unsigned int p_line);
	vector<LineCollisionData> LineSortedCollisions(unsigned int p_line);
	int					 LineClosestCollision(unsigned int p_line);

	void ActivateBody(unsigned int pBody);
	void InactivateBody(unsigned int pBody);
	bool IsActive(unsigned int pBody);

	void InitStaticBodiesOctree(AglVector3 pMin, AglVector3 pMax);
};

#endif