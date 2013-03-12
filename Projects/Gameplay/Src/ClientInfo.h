#pragma once

#include <Component.h>
// =======================================================================================
//                                      ClientInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains client specific information
///        
/// # ClientInfo
/// Detailed description.....
/// Created on: 9-1-2013 
///---------------------------------------------------------------------------------------

class ClientInfo : public Component
{
public:
	ClientInfo(int p_id);
public:
	int		id;
	float	ping;
	int		score;
};