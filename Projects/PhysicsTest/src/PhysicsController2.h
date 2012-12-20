#ifndef PHYSICSCONTROLLER2_H
#define PHYSICSCONTROLLER2_H

#include <ImpulseSolver.h>
#include "DebugBox.h"
#include "DebugHull.h"
#include "DebugSphere.h"
#include <PhysicsController.h>

// =================================================================================================
//                                         PhysicsController
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Contains control and update logic for the physics system
///
/// 
/// Edited By: Anton Andersson, 2012-11-26
///-------------------------------------------------------------------------------------------------
class PhysicsController2
{
private:
	ID3D11Device*			mDevice; ///< Pointer to device. Should be removed
	ID3D11DeviceContext*	mDeviceContext; ///< Pointer to device context. Should be removed


	PhysicsController*			mController;

	vector<ConvexHullShape*>	mConvexHullShapes; ///< Debug: Hull Shapes
	DebugBox*					mDebugBox; ///< Debug: Box Shape
	DebugSphere*				mDebugSphere; ///<Debug Sphere Shape
	vector<DebugHullData*>		mDebugHullData; ///< Hull Data used by debug hulls

	float mTimeAccum;

public:

	///-----------------------------------------------------------------------------------
	/// Constructor
	/// \param p_device Dx11 Device
	/// \param p_deviceContext Dx11 DeviceContext
	///-----------------------------------------------------------------------------------
	PhysicsController2(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext);

	///-----------------------------------------------------------------------------------
	/// Destructor
	///-----------------------------------------------------------------------------------
	virtual ~PhysicsController2();

	///-----------------------------------------------------------------------------------
	/// Adds a sphere to the set of rigid bodies
	/// \param p_position Position of the sphere
	/// \param p_radius Radius of the sphere
	/// \param p_userControlled Bool indicating whether or not the sphere is user controlled
	/// \return Returns the sphere that was added.
	///-----------------------------------------------------------------------------------
	int AddSphere(AglVector3 p_position, float p_radius, bool p_userControlled = false, CompoundBody* p_parent = NULL);

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
	int AddBox(AglVector3 p_position, AglVector3 p_size, float p_mass, AglVector3 p_velocity, AglVector3 p_angularVelocity, bool p_static = false, CompoundBody* p_parent = NULL);

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
	int AddConvexHull(AglVector3 p_position, float p_size, float p_mass, AglVector3 p_velocity, AglVector3 p_angularVelocity, bool p_static = false);

	int AddCompoundBody(AglVector3 p_position);

	int AddMeshBody(AglMatrix pCoordinateSystem, AglVector3 pPosition, AglOBB pOBB, AglBoundingSphere pBoundingSphere, AglLooseBspTree* pBSPTree = NULL,
		AglInteriorSphereGrid* pSphereGrid = NULL);

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
	/// Draws Debug information
	///-----------------------------------------------------------------------------------
	void DrawDebug();


	///-----------------------------------------------------------------------------------
	/// Checks a set of rays against the set of rigid bodies and returns the closest collision
	/// \param p_rays The rays
	/// \param p_ignore A rigid body to ignore
	/// \return Time of the closest collision
	///-----------------------------------------------------------------------------------
	float RaysVsObjects(vector<PhyRay> p_rays, RigidBody* p_ignore, AglBoundingSphere p_sphere);

	void DetachBodyFromCompound(CompoundBody* p_compound, RigidBody* p_body);

	Body* GetBody(int pIndex){ return mController->getBody(pIndex);}
};

#endif