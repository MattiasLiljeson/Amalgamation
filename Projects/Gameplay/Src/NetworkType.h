// =======================================================================================
//	NetworkType
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # NetworkType
/// Detailed description...
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

class NetworkType
{
public:
	enum Network_T
	{
		NON_EXISTENT = -1,
		Ship,
		Prop,
		Identity,
		Other,
		// Johan: We could probably remove this whole enum.
	};

public:
	Network_T type;

};