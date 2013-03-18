#pragma once
#include <Component.h>

// =======================================================================================
//                                 RegisteredEntity
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	An entity id that another entity can keep track of
///        
/// # RegisteredEntity
/// Detailed description.....
/// Created on: 21-1-2013 
///---------------------------------------------------------------------------------------

class RegisteredEntity : public Component
{
public:
	RegisteredEntity(int p_id)
		: Component( ComponentType::RegisteredEntity )
	{
		m_entityId=p_id;
	}

	int m_entityId;
};