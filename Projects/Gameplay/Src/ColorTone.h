#pragma once

#include <Component.h>
#include <ComponentFactory.h>
#include <AglVector4.h>

// =======================================================================================
//                                      ColorTone
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	General color toning information for meshes
///        
/// # ColorTone
/// Detailed description.....
/// Created on: 1-3-2013 
///---------------------------------------------------------------------------------------

class ColorTone : public Component
{
public:
	ColorTone();
	ColorTone(AglVector4 p_color, bool p_enabled);
	~ColorTone();

public:
	AglVector4 color;
	bool toneEnabled;
};