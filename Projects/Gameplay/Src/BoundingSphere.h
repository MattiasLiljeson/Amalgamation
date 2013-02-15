// =======================================================================================
//                                      BoundingSphere
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Used for culling
///        
/// # BoundingSphere
/// Detailed description.....
/// Created on: 14-02-2013 
///---------------------------------------------------------------------------------------
#pragma once
#include <Component.h>
#include <AglBoundingSphere.h>

class BoundingSphere : public Component
{
public:
	BoundingSphere(AglBoundingSphere p_sphere): Component(ComponentType::BoundingSphere)
	{
		sphere = p_sphere;
	}
	virtual ~BoundingSphere()
	{

	}

	AglBoundingSphere sphere;
};