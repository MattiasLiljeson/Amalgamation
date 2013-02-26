#pragma once

class AglVector3;
class AglVector4;
class AglMatrix;
// =======================================================================================
//                                      ColourSpaceConverter
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Utility class for converting a colour space to another
///        
/// # ColourSpaceConverter
/// Detailed description.....
/// Created on: 26-2-2013 
///---------------------------------------------------------------------------------------

class ColourSpaceConverter
{
public:
	static void transformToSRGB(AglVector4& p_colour);
	static void transformToSRGB(AglVector3& p_colour);
	static void transformToRGB(AglVector4& p_colour);
	static void transformToRGB(AglVector3& p_colour);
	static void applyGammaCorrection(AglVector3& p_colour);
protected:
private:
	static const AglMatrix m_CIEXYZToSRGB;
	static const AglMatrix m_CIEXYZToSRGBReverse;
};