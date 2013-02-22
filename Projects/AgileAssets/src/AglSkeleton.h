#ifndef AGLSKELETON_H
#define AGLSKELETON_H

#include "Agl.h"
#include <vector>

using namespace std;


class AglScene;

// =================================================================================================
//                                         AglSkeletonHeader
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Header fo an AglSkeleton.
///
/// 
/// # AglSkeletonHeader
/// Edited By: Anton Andersson, 2012-11-16
///-------------------------------------------------------------------------------------------------
struct AglSkeletonHeader
{
	int			nameID;		///< Index to the name of the skeleton
	int			jointCount;	///< The number of the joints in the skeleton
	AglMatrix	baseTransform; ///< Acts as an extra transform above the root node. 
							   ///< Useful for coordinate switches
};

// =================================================================================================
//                                         AglJoint
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief A joint is simply a node in a tree with an index to a node.
///
/// 
/// # AglJoint
/// Several joints together form a hierarchy that creates a skeleton. Each joint can collect
/// its transform from the corresponding AglNode.
/// Edited By: Anton Andersson, 2012-11-16
///-------------------------------------------------------------------------------------------------
struct AglJoint
{
	int		id;
	int		nodeID;
	int		parentIndex;
};

// =================================================================================================
//                                         AglSkeleton
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief A skeleton used for skeletal animation
///
/// 
/// # AglSkeleton
/// The AglSkeleton is a hierarchy of joints that together
/// form a skeleton. 
/// Edited By: Anton Andersson, 2012-11-16
///-------------------------------------------------------------------------------------------------
class AglSkeleton
{
private:
	AglSkeletonHeader	m_header;	///< Header
	AglJoint*			m_joints;	///< List of joints
	AglScene*			m_scene;	///< Pointer to the Scene context

public:
	///
	/// Constructor
	/// \param p_header Header.
	/// \param p_joints List of joints.
	/// \param p_scene Pointer to the scene context.
	///
	AglSkeleton(AglSkeletonHeader p_header, AglJoint* p_joints, AglScene* p_scene);
	///
	/// Destructor
	///
	~AglSkeleton();

	/// 
	/// Gets the header for the skeleton.
	/// \return The header
	///
	AglSkeletonHeader getHeader();

	///
	/// Gets the joint list.
	/// \return A list of joints.
	///
	AglJoint*	  getJoints();

	///
	/// Gets the root joint.
	/// \return The root joint.
	///
	AglJoint	 getRoot();

	vector<AglJoint> getChildren(int p_parent);

	///
	/// Gets the global transform of a joint.
	/// \param p_joint The index to the joint.
	/// \return A 4x4 matrix
	///
	AglMatrix	  getGlobalTransform(int p_joint);

	AglMatrix	  getGlobalTransform(int p_joint, float p_time);

	///
	///Gets the inverse bind transform of a joint.
	/// \param p_joint The index to the joint.
	/// \return A 4x4 matrix
	///
	AglMatrix	  getInverseBindMatrix(int p_joint);
};

#endif