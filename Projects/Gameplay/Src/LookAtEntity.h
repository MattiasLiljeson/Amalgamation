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
		float p_moveSpeed=1.0f,float p_rotationSpeed=1.0f)
		: Component( ComponentType::LookAtEntity )
	{
		m_entityId = p_entityId;
		m_followPositionOffset = AglVector3::zero();
		m_orbitRotationOffset = AglQuaternion::identity();
		m_moveSpd = p_moveSpeed;
		m_rotationSpd = p_rotationSpeed;
	}

	LookAtEntity(int p_entityId,
		const AglVector3& p_followPositionOffset,
		float p_moveSpeed=1.0f,float p_rotationSpeed=1.0f
		)
		: Component( ComponentType::LookAtEntity )
	{
		m_entityId = p_entityId;
		m_followPositionOffset = p_followPositionOffset;
		m_orbitRotationOffset = AglQuaternion::identity();
		m_moveSpd = p_moveSpeed;
		m_rotationSpd = p_rotationSpeed;
	}

	LookAtEntity(int p_entityId,
				const AglVector3& p_followPositionOffset,
				const AglQuaternion& p_orbitRotationOffset,
				float p_moveSpeed=1.0f,float p_rotationSpeed=1.0f,
				float p_orbitDistance=1.0f
				)
		: Component( ComponentType::LookAtEntity )
	{
		m_entityId = p_entityId;
		m_followPositionOffset = p_followPositionOffset;
		m_orbitRotationOffset = p_orbitRotationOffset;
		m_moveSpd = p_moveSpeed;
		m_rotationSpd = p_rotationSpeed;
		m_orbitDistance = p_orbitDistance;
	}

	~LookAtEntity() {}

	const AglVector3& getFollowPositionOffset() {return m_followPositionOffset;}
	const AglQuaternion& getOrbitRotationOffset() {return m_orbitRotationOffset;}
	float getOrbitDistance() {return m_orbitDistance;}
	int getEntityId() {return m_entityId;}
	float getRotationSpeed() {return m_rotationSpd;}
	float getMoveSpd() {return m_moveSpd;}

	void setFollowPositionOffset(const AglVector3& p_offset) {m_followPositionOffset=p_offset;}
	void setOrbitRotationOffset(const AglQuaternion& p_offset) {m_orbitRotationOffset=p_offset;}
	void setOrbitDistance(float p_orbitDistance) {m_orbitDistance = p_orbitDistance;}
	void setRotationSpeed(float p_val) {m_rotationSpd=p_val;}
	void setMoveSpeed(float p_val) {m_moveSpd=p_val;}	
	
	AglVector3 m_oldPos;
private:
	int m_entityId;
	AglVector3 m_followPositionOffset; ///< offset position from target
	AglQuaternion m_orbitRotationOffset; ///< offset rotation from target
	float m_rotationSpd; ///< Speed of rotation towards goal
	float m_moveSpd; ///< Speed of movement towards goal  
	float m_orbitDistance; ///< distance from target when orbiting

};

