#pragma once

// =======================================================================================
//                                      LookAtEntity
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Store reference id to look-at target as well as an offset for the observer.
///        
/// # LookAtEntity
/// Detailed description.....
/// Created on: 14-12-2012 
///---------------------------------------------------------------------------------------

#pragma once

#include "Component.h"
#include <AglVector3.h>
#include <GraphicsWrapper.h>
#include <RendererSceneInfo.h>

class LookAtEntity : public Component
{
public:
	LookAtEntity(int p_entityId, AglVector3 p_observerOffset)
	{
		m_entityId = p_entityId;
		m_observerOffset = p_observerOffset;
	}

	~LookAtEntity() {}

	const AglVector3& getOffset() {return m_observerOffset;}
	int getEntityId() {return m_entityId;}

private:
	int m_entityId;
	AglVector3 m_observerOffset;
};

