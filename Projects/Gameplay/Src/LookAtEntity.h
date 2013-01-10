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
	LookAtEntity(int p_entityId,
				const AglVector3& p_observerPositionalOffset,
				const AglQuaternion& p_observerRotationalOffset,
				float p_moveSpeed=1.0f,float p_rotationSpeed=1.0f
				)
	{
		m_entityId = p_entityId;
		m_observingPositionalOffset = p_observerPositionalOffset;
		m_observingRotationalOffset = p_observerRotationalOffset;
		m_moveSpd = p_moveSpeed;
		m_rotationSpd = p_rotationSpeed;
	}

	~LookAtEntity() {}

	const AglVector3& getObservingPositionalOffset() {return m_observingPositionalOffset;}
	const AglQuaternion& getObservingRotationalOffset() {return m_observingRotationalOffset;}
	int getEntityId() {return m_entityId;}
	float getRotationSpeed() {return m_rotationSpd;}
	float getMoveSpd() {return m_moveSpd;}

	void setObservingPositionalOffset(const AglVector3& p_offset) {m_observingPositionalOffset=p_offset;}
	void setObservingRotationalOffset(const AglQuaternion& p_offset) {m_observingRotationalOffset=p_offset;}
	void setRotationSpeed(float p_val) {m_rotationSpd=p_val;}
	void setMoveSpeed(float p_val) {m_moveSpd=p_val;}
private:
	int m_entityId;
	AglVector3 m_observingPositionalOffset; ///< offset position from target
	AglQuaternion m_observingRotationalOffset; ///< offset rotation from target
	float m_rotationSpd; ///< Speed of rotation towards goal
	float m_moveSpd; ///< Speed of movement towards goal    
};

