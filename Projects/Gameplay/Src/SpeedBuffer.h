#pragma once
#include <Component.h>
#include <NumericBuffer.h>
#include <AglVector3.h>
#include <ComponentFactory.h>



class SpeedBuffer : public Component
{
public:
	static ComponentRegister<SpeedBuffer> s_reg;

	SpeedBuffer();
	virtual ~SpeedBuffer();

	NumericBuffer<float, 25> m_buffer;
	AglVector3 m_oldPos;
};

