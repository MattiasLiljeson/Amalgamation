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
	string m_name; ///< Name of the gradient. Should be moved
	int m_gradientTextureNameIndex; ///< Index to the gradient texture name
	int m_normalTextureNameIndex; ///< Index to the normal texture name
	vector<AglGradientMaterial*> m_layers; //< List of layers of GradientMaterial
public:
	///
	/// Constructor
	/// \param p_name Name of the Gradient.
	///
	AglGradient(string p_name);

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
	/// Gets the name index of the gradient texture
	/// \return The index
	///
	int getGradientTextureNameIndex();
	
	///
	/// Set the name index of the gradient texture
	/// \param The new index
	///
	void setGradientTextureNameIndex(int pIndex);

	///
	/// Gets the name index of the normal texture
	/// \return The index
	///
	int getNormalTextureNameIndex();

	///
	/// Sets the name index of the normal texture
	/// \param The new index
	///
	void setNormalTextureNameIndex(int pIndex);

	///
	/// Adds a layer to the list of layers
	/// \param The new layer.
	///
	void addLayer(AglGradientMaterial* pLayer);

};

#endif