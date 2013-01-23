// =======================================================================================
//	EntityType
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # NetworkType
/// Detailed description...
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

class EntityType
{
public:
	enum EntityEnums
	{
		NON_EXISTENT = -1,
		Ship,
		Prop,
		StaticProp,
		Identity,
		ShipModule,
		ParticleSystem,
		Other,
	};

public:
	EntityEnums type;
};