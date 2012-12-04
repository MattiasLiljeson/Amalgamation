// =======================================================================================
//                                      AglOBB
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # OBB
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#ifndef AGLOBB_H
#define AGLOBB_H

#include "AglVector3.h"

class AglOBB
{
	AglVector3 position;
	AglVector3 size;
	AglVector3 axes[3];

	AglOBB(AglVector3 p_position, AglVector3 p_size);
};

#endif