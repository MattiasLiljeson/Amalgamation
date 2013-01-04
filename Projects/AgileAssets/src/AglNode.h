#ifndef AGLNODE_H
#define AGLNODE_H

//TODO: Merge AglNode an AglDynamic node into one
//node in which only the static data gets sent to
//files on export. Reason for delay is that too many 
//changes at once may cause hard to find bugs

// =================================================================================================
//                                         AglNode
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Node in the scene used to transform vertices in a joint hierarchy
///
/// 
/// # AglNode
/// Edited By: Anton Andersson, 2012-11-16
///-------------------------------------------------------------------------------------------------
struct AglNode
{
	int			id;
	AglMatrix	inverseBindMatrix;
	AglMatrix	localTransform;
};

// =================================================================================================
//                                         AglDynamicNode
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Dynamic Node used to store an updatable transform.
///
/// 
/// # AglNode
// Handles animated nodes that uses several animation layers to be able to blend.
/// Edited By: Anton Andersson, 2012-11-16
///-------------------------------------------------------------------------------------------------
struct AglDynamicNode
{
	AglMatrix transform;
	bool animated;
};

#endif