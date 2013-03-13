#define FORCE_VS_DBG_OUTPUT
#include "OutputLogger.h"
#include <DebugUtil.h>

bool OutputLogger::checkProcessing()
{
	return false;
}

void OutputLogger::write( const char* p_msg, WriteType p_type /*= WRITETYPE_INFO*/ )
{
#ifndef FORCE_DISABLE_OUTPUT
	if (m_outStream.is_open())
	{
		if (p_type == WRITETYPE_WARNING)
		{
			m_outStream << "Warning: ";
			DEBUGWARNING((p_msg));
		}
		else if (p_type == WRITETYPE_ERROR)
		{
			m_outStream << "Error: ";
			DEBUGWARNING((p_msg));
		}
		else
		{
			m_outStream << "Info: ";
			DEBUGPRINT((p_msg));
		}
		m_outStream << p_msg;
	}
#endif
}

OutputLogger::OutputLogger( const char* p_fileName )
	: EntitySystem(SystemType::LogToFileSystem)
{
#ifndef FORCE_DISABLE_OUTPUT
	m_outStream.open(p_fileName, std::ios_base::out | std::ios_base::trunc);
#endif
}

OutputLogger::~OutputLogger()
{
#ifndef FORCE_DISABLE_OUTPUT
	if (m_outStream.is_open())
		m_outStream.close();
#endif
}

