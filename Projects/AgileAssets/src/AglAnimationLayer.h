#ifndef AGLANIMATIONLAYER_H
#define AGLANIMATIONLAYER_H

#include "Agl.h"

class AglScene;

// =================================================================================================
//                                         AglAnimationLayerHeader
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Header of an AglAnimationLayer
///
/// 
/// # AglAnimationLayerHeader
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
struct AglAnimationLayerHeader
{
	int id;					///< Index of the layer
	unsigned int nodeCount; ///< Number of nodes in the layer
};

// =================================================================================================
//                                         AglAnimationLayer
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Specifies an animation layer used to blend animations. CLASS INCOMPLETE!
///
/// 
/// # AglAnimationLayer
/// An animationlayer contains a certain amount of animation data. It can be paired
/// simultaneoulsy with other animation layers in order to create blended animations
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------
class AglAnimationLayer
{
private:
	AglAnimationLayerHeader m_header; ///< AnimationLayer header
	unsigned int* m_nodes; ///< List of indices to the node animations controlled by the layer
	AglScene* m_scene; ///< Pointer to the scene context
public:
	///
	/// Constructor
	/// \param p_header AnimationLayer Header
	/// \param p_nodes List of indices to the affected node animations
	/// \param p_scene Pointer to the scene context
	///
	AglAnimationLayer(AglAnimationLayerHeader p_header, unsigned int* p_nodes, AglScene* p_scene);

	///
	/// Destructor
	///
	~AglAnimationLayer();

	///
	/// Returns the AnimationLayer Header
	/// \return The Animation Layer Header
	///
	AglAnimationLayerHeader getHeader();

	///
	/// Get the node animation indices
	/// \return The node animations affected by the layer
	///
	unsigned int* getNodes();

	///
	/// Evaluate the transforms of the layer at a certain time
	/// \param p_time The time at which to evaluate the layer
	///
	void evaluate(float p_time);

	AglMatrix evaluate(float p_time, unsigned int p_node);

	///
	/// Get the minimum time of the animation layer
	/// \return The minimum time of the animation layer
	///
	float getMinTime();

	///
	/// Get the maximum time of the animation layer
	/// \return The maximum time of the animation layer
	///
	float getMaxTime();
};

#endif