#include "ColourSpaceConverter.h"
#include "AglMatrix.h"
#include "AglVector4.h"
#include "AglVector3.h"

const AglMatrix ColourSpaceConverter::m_CIEXYZToSRGB(
	3.24060f,	-1.5372f,	-0.4986f,	0.0f,
	-0.9689f,	1.8758f,	0.0415f,	0.0f,
	0.0557f,	-0.2040f,	1.0570f,	0.0f,
	0.0f,		0.0f,		0.0f,		1.0f); ///< http://en.wikipedia.org/wiki/SRGB

const AglMatrix ColourSpaceConverter::m_CIEXYZToSRGBReverse(
	0.4124f,	-0.3576f,	-0.1805f,	0.0f,
	-0.2126f,	0.7152f,	0.0722f,	0.0f,
	0.0193f,	-0.1192f,	0.9505f,	0.0f,
	0.0f,		0.0f,		0.0f,		1.0f); ///< http://en.wikipedia.org/wiki/SRGB

void ColourSpaceConverter::transformToSRGB( AglVector4& p_colour )
{
	p_colour.transform(m_CIEXYZToSRGB);
}

void ColourSpaceConverter::transformToSRGB( AglVector3& p_colour )
{
	p_colour.transform(m_CIEXYZToSRGB);
}

void ColourSpaceConverter::transformToRGB( AglVector4& p_colour )
{
	p_colour.transform(m_CIEXYZToSRGBReverse);
}

void ColourSpaceConverter::transformToRGB( AglVector3& p_colour )
{
	p_colour.transform(m_CIEXYZToSRGBReverse);
}

void ColourSpaceConverter::applyGammaCorrection( AglVector3& p_colour )
{
	p_colour.x = pow(p_colour.x,1/2.2f);
	p_colour.y = pow(p_colour.y,1/2.2f);
	p_colour.z = pow(p_colour.z,1/2.2f);
}
