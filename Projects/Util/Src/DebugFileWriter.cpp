#include "DebugFileWriter.h"

void DebugFileWriter::writeToFile( const char* p_msg, WriteType p_type )
{
	if (m_initialised)
	{
		if (p_type == WRITETYPE_WARNING)
			m_outStream << "Warning: ";
		else if (p_type == WRITETYPE_ERROR)
			m_outStream << "Error: ";
		m_outStream << p_msg;
	}
}

DebugFileWriter::~DebugFileWriter()
{
	if (m_outStream.is_open())
		m_outStream.close();
}

DebugFileWriter::DebugFileWriter()
{
	m_outStream.open("log.txt", std::ios_base::out | std::ios_base::trunc);
	m_initialised = true;
}


