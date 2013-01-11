#pragma once
#include <Component.h>
#include <AglVector3.h>
#include <AglQuaternion.h>

class BodyInitData : public Component
{
public:
	// Added these enums to make the large constructor clearer.
	// Another solution might be more appropriate though? -Jarl
	enum CompoundMode
	{
		SINGLE,
		COMPOUND
	};

	enum ResponseMode
	{
		DYNAMIC,
		STATIC
	};

	BodyInitData(AglVector3 p_position, AglQuaternion p_orientation,
					AglVector3 p_scale, AglVector3 p_velocity,
					AglVector3 p_angularVelocity, int p_type,
					ResponseMode p_responseMode = ResponseMode::DYNAMIC,
					CompoundMode p_compoundMode = CompoundMode::SINGLE, bool p_impulseEnabled = true,
					bool p_collisionEnabled = true)
	{
		m_position = p_position;
		m_orientation = p_orientation;
		m_scale = p_scale;
		m_velocity = p_velocity;
		m_angularVelocity = p_angularVelocity;
		m_type = p_type;

		m_static = p_responseMode==ResponseMode::STATIC; // change these?
		m_compound = p_compoundMode==CompoundMode::COMPOUND;
		m_impulseEnabled = p_impulseEnabled;
		m_collisionEnabled = p_collisionEnabled;
	}
	AglVector3 m_position;
	AglQuaternion m_orientation;
	AglVector3 m_scale;
	AglVector3 m_velocity;
	AglVector3 m_angularVelocity;

	int m_type;

	bool m_static;

	bool m_compound;

	bool m_impulseEnabled;

	bool m_collisionEnabled;
};