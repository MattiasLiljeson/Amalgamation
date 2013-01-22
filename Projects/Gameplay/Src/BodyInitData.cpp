#include "BodyInitData.h"
#include <ComponentFactory.h>

ComponentRegister<BodyInitData> BodyInitData::s_reg("BodyInitData");

void BodyInitData::init( vector<ComponentData> p_initData )
{
	for( unsigned int i=0; i<p_initData.size(); i++ )
	{
		if( p_initData[i].dataName == "m_posX" )
			p_initData[i].getData<float>(&m_position.x);
		else if( p_initData[i].dataName == "m_posY" )
			p_initData[i].getData<float>(&m_position.y);
		else if( p_initData[i].dataName == "m_posZ" )
			p_initData[i].getData<float>(&m_position.z);

		else if( p_initData[i].dataName == "m_scaleX" )
			p_initData[i].getData<float>(&m_scale.x);
		else if( p_initData[i].dataName == "m_scaleY" )
			p_initData[i].getData<float>(&m_scale.y);
		else if( p_initData[i].dataName == "m_scaleZ" )
			p_initData[i].getData<float>(&m_scale.z);

		AglVector3 u = m_orientation.u;
		if( p_initData[i].dataName == "m_rotationX" )
			p_initData[i].getData<float>(&u.x);
		else if( p_initData[i].dataName == "m_rotationY" )
			p_initData[i].getData<float>(&u.x);
		else if( p_initData[i].dataName == "m_rotationZ" )
			p_initData[i].getData<float>(&u.x);
		m_orientation.u = u;
		if( p_initData[i].dataName == "m_rotationW" )
			p_initData[i].getData<float>(&m_orientation.v);

		if( p_initData[i].dataName == "m_velocityX" )
			p_initData[i].getData<float>(&m_velocity.x);
		else if( p_initData[i].dataName == "m_velocityY" )
			p_initData[i].getData<float>(&m_velocity.y);
		else if( p_initData[i].dataName == "m_velocityZ" )
			p_initData[i].getData<float>(&m_velocity.z);

		else if( p_initData[i].dataName == "m_angularVelocityX" )
			p_initData[i].getData<float>(&m_angularVelocity.x);
		else if( p_initData[i].dataName == "m_angularVelocityY" )
			p_initData[i].getData<float>(&m_angularVelocity.y);
		else if( p_initData[i].dataName == "m_angularVelocityZ" )
			p_initData[i].getData<float>(&m_angularVelocity.z);

		else if( p_initData[i].dataName == "m_angularVelocityX" )
			p_initData[i].getData<float>(&m_angularVelocity.x);
		else if( p_initData[i].dataName == "m_angularVelocityY" )
			p_initData[i].getData<float>(&m_angularVelocity.y);
		else if( p_initData[i].dataName == "m_angularVelocityZ" )
			p_initData[i].getData<float>(&m_angularVelocity.z);

		else if( p_initData[i].dataName == "m_type" )
			p_initData[i].getData<int>(&m_type);
		else if( p_initData[i].dataName == "m_static" )
			p_initData[i].getData<bool>(&m_static);
		else if( p_initData[i].dataName == "m_compound" )
			p_initData[i].getData<bool>(&m_compound);
		else if( p_initData[i].dataName == "m_impulseEnabled" )
			p_initData[i].getData<bool>(&m_impulseEnabled);
		else if( p_initData[i].dataName == "m_collisionEnabled" )
			p_initData[i].getData<bool>(&m_collisionEnabled);

	}
}