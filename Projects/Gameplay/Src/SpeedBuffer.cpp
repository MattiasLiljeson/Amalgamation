#include "SpeedBuffer.h"

ComponentRegister<SpeedBuffer> SpeedBuffer::s_reg("SpeedBuffer");

SpeedBuffer::SpeedBuffer() :
	Component( ComponentType::SpeedBuffer )
{
	m_buffer.setAllVals( 0.0f );
	m_oldPos.x = 0.0f;
	m_oldPos.y = 0.0f;
	m_oldPos.z = 0.0f;
}


SpeedBuffer::~SpeedBuffer()
{
}
