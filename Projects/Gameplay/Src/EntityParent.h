#pragma once
#include <Component.h>
#include <AglMatrix.h>

// =======================================================================================
//                                      TransformParent
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Base transform on a parent entity
///        
/// # TransformParent
/// Detailed description.....
/// Created on: 21-1-2013 
///---------------------------------------------------------------------------------------

class EntityParent : public Component
{
public:
	EntityParent(int p_entityId, const AglMatrix& p_localTransform) 
		: m_entityId(p_entityId), m_localTransform(p_localTransform) {}

	const AglMatrix& getLocalTransform() {return m_localTransform;}
	int getParentEntityId() {return m_entityId;}
private:
	int m_entityId;
	AglMatrix m_localTransform;
};