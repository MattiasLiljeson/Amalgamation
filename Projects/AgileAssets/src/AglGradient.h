// =================================================================================================
//                                         AglGradient
// =================================================================================================

///-------------------------------------------------------------------------------------------------
/// \brief Specifies a gradient used to shade an object
///
/// 
/// # AglGradient
/// A gradient uses a gradient texture instead of ordinary
/// diffuse/speculare etc textures. Through this gradient texture
/// the gradient shades an object using materials indexed by the texture.
///	details are achieved through normal and displacement mapping
/// Edited By: Anton Andersson, 2012-11-20
///-------------------------------------------------------------------------------------------------

#ifndef GRADIENT_H
#define GRADIENT_H

#include "Agl.h"
#include <vector>

using namespace std;

class AglGradient
{
private:
	vector<AglGradientMaterial*> m_layers; //< List of layers of GradientMaterial
public:
	///
	/// Constructor
	/// \param p_name Name of the Gradient.
	///
	AglGradient();

	///
	/// Destructor
	///
	virtual ~AglGradient();

	///
	/// Return the layers of the Gradient.
	/// \return The layers.
	///
	vector<AglGradientMaterial*> getLayers();

	///
	/// Adds a layer to the list of layers
	/// \param The new layer.
	///
	void addLayer(AglGradientMaterial* pLayer);

	AglVector4* getLayerColorPointer(int p_index)
	{
		return &m_layers[p_index]->color;
	}

};

#endif