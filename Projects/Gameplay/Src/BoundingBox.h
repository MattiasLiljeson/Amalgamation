// =======================================================================================
//                                      BoundingBox
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Used for culling
///        
/// # BoundingBox
/// Detailed description.....
/// Created on: 17-02-2013 
///---------------------------------------------------------------------------------------
#pragma once
#include <Component.h>
#include <AglOBB.h>

class BoundingBox : public Component
{
public:
	BoundingBox(AglOBB p_box): Component(ComponentType::BoundingBox)
	{
		box = p_box;
	}
	virtual ~BoundingBox()
	{

	}

	AglOBB box;
};