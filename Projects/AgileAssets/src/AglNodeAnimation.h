#ifndef AGLNODEANIMATION_H
#define AGLNODEANIMATION_H

#include "Agl.h"

class AglScene;

// =================================================================================================
//                                         AglNodeAnimationHeader
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Header to a certain AglNodeAnimation
///
/// 
/// # AglNodeAnimationHeader
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
struct AglNodeAnimationHeader
{
	int id;						///< Index of the AglNodeAnimation
	unsigned int targetNode;	///< Index to the target node of the animation
	unsigned int keyFrameCount;	///< Number of keyframes the animation contains
};

// =================================================================================================
//                                         AglKeyFrame
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Keyframe of a certain transform. Usually applied to a node
///
/// 
/// # AglKeyFrame
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
struct AglKeyFrame
{
	float time;				///< The time at which the keyframe occurs
	AglMatrix transform;	///< The transform at the given time.
};

// =================================================================================================
//                                         AglNodeAnimation
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Specifies the transformations of a certain node during animation
///
/// 
/// # AglNodeAnimation
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
class AglNodeAnimation
{
private:
	AglNodeAnimationHeader	m_header;		///< The header
	AglKeyFrame*			m_keyFrames;		///< List of keyframes
	AglScene*				m_scene;			///< Pointer to the scene context
public:

	///
	/// Constructor
	/// \param p_header Node Animation Header
	/// \param p_keyFrames List of all the key frame for the animation
	/// \param p_scene Pointer to the scene context
	///
	AglNodeAnimation(AglNodeAnimationHeader p_header, AglKeyFrame* p_keyFrames, AglScene* p_scene);

	///
	/// Destructor
	///
	~AglNodeAnimation();

	///
	/// Returns the header of the node animation
	/// \return The Header
	///
	AglNodeAnimationHeader getHeader();

	///
	/// Returns the list of key frames
	/// \return The list of key frames
	///
	AglKeyFrame* getKeyFrames();

	///
	/// Appends a transform to the related node
	/// \param p_time The time at which to get the transform
	/// \param p_weight The weight this transform will be given in relation to the entire transform.
	///
	void appendTransform(float p_time, float p_weight);

	AglMatrix evaluate(float p_time);

	///
	///Returns the minimum time of the animation sequence
	///\return The minimum time
	///
	float getMinTime();

	///
	///Returns the maximum time of the animation sequence
	///\return The maximum time
	///
	float getMaxTime();
};

#endif