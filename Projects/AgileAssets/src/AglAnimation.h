#ifndef AGLANIMATION_H
#define AGLANIMATION_H

#include "Agl.h"

class AglScene;

// =================================================================================================
//                                         AglAnimationHeader
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Header of an AglAnimation
///
/// 
/// # AglAnimationHeader
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
struct AglAnimationHeader
{
	int id;						///< Index of the animation
	unsigned int layerCount;	///< The number of layers in the animation
};

// =================================================================================================
//                                         AglAnimation
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Specifies an animation made up of AglAnimationLayers
///
/// 
/// # AglAnimation
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
class AglAnimation
{
private:
	AglAnimationHeader	m_header; ///< Animation header
	unsigned int*		m_layers; ///< List of indices to the layers affected by the animation.
	bool				m_isPlaying; ///< Indicates whether or not the animation is playing
	float				m_currentTime; ///< Current time of the animation
	AglScene*			m_scene; ///< Pointer to the scene context
						
	bool				m_limitsSet; ///< Indicates whether or not the max and min values have been set
	float				m_maxTime; ///< Maximum time of the animation
	float				m_minTime; ///< Minimum time of the animation
private:
	///
	/// Determine the max and min times of the animation
	///
	void determineTimeLimits();
public:
	///
	/// Constructor
	/// \param p_header Animation Header
	/// \param p_layers List of indices to the layers
	/// \param p_scene Pointer to the scene context
	///
	AglAnimation(AglAnimationHeader p_header, unsigned int* p_layers, AglScene* p_scene);

	///
	/// Destructor
	///
	~AglAnimation();

	///
	/// Get the AnimationHeader of the Animation
	/// \return The animation header
	///
	AglAnimationHeader	getHeader();

	///
	/// Get the indices to the layers of the Animation
	/// \return The layer indices
	///
	unsigned int*		getLayers();

	///
	/// Update the animation
	/// \param p_dt The timestep
	///
	void				update(float p_dt);

	///
	/// Play the animation. Sets the m_isPlaying flag
	///
	void				play();
};

#endif