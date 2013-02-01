#pragma once
#include <Component.h>
#include <AglVector3.h>
#include <AglQuaternion.h>
#include <ComponentFactory.h>
#include "ModelResource.h"

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
	enum BodyType
	{
		BOX,
		SPHERE,
		MESH,
		BOXFROMMESHOBB
	};

	BodyInitData();

	BodyInitData(AglVector3 p_position, AglQuaternion p_orientation,
					AglVector3 p_scale, AglVector3 p_velocity,
					AglVector3 p_angularVelocity, int p_type,
					ResponseMode p_responseMode = ResponseMode::DYNAMIC,
					CompoundMode p_compoundMode = CompoundMode::SINGLE, bool p_impulseEnabled = true,
					bool p_collisionEnabled = true);

	virtual void init( vector<ComponentData> p_initData );

	AglVector3 m_position;
	AglQuaternion m_orientation;
	AglVector3 m_scale;
	AglVector3 m_velocity;
	AglVector3 m_angularVelocity;

	ModelResource* m_modelResource;

	int m_type;

	bool m_static;

	bool m_compound;

	bool m_impulseEnabled;

	bool m_collisionEnabled;
private:
	static ComponentRegister<BodyInitData> s_reg;
};