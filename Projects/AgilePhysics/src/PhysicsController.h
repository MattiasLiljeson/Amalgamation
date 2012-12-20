#ifndef PHYSICSCONTROLLER_H
#define PHYSICSCONTROLLER_H

#include "ImpulseSolver.h"
#include "CompoundBody.h"
#include "RigidBodyMesh.h"
#include <AglInteriorSphereGrid.h>
#include <AglLooseBspTree.h>

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
	vector<RigidBody*> mRigidBodies; ///< List of rigid bodies
	vector<CompoundBody*> mCompoundBodies; ///<List of compound bodies

	vector<ConvexHullShape*>	mConvexHullShapes; ///< Debug: Hull Shapes

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
	int AddBox(AglVector3 p_position, AglVector3 p_size, float p_mass, AglVector3 p_velocity, AglVector3 p_angularVelocity, bool p_static = false, CompoundBody* pParent = NULL);

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

	int AddMeshBody(AglMatrix pCoordinateSystem, AglVector3 pPosition, AglOBB pOBB, AglBoundingSphere pBoundingSphere, AglLooseBspTree* pBSPTree = NULL,
		AglInteriorSphereGrid* pSphereGrid = NULL);

	void DetachBodyFromCompound(CompoundBody* p_compound, RigidBody* p_body);

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

	vector<RigidBody*> getBodies(){ return mRigidBodies; }

	Body* getBody(int pIndex){ return mBodies[pIndex]; }

	void ApplyExternalImpulse(int p_id, AglVector3 p_impulse, AglVector3 p_angularImpulse);
};

#endif