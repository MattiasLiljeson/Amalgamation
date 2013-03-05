#pragma once

#include <fstream>
using namespace std;

enum WriteType
{
	WRITETYPE_UNKNOWN,
	WRITETYPE_WARNING,
	WRITETYPE_ERROR
};

// =======================================================================================
//                                      DebugFileWriter
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # DebugFileWriter
/// Detailed description.....
/// Created on: 5-3-2013 
///---------------------------------------------------------------------------------------


class DebugFileWriter
{
public:
	void writeToFile(const char* p_msg, WriteType p_type = WRITETYPE_UNKNOWN);

	DebugFileWriter();
	~DebugFileWriter();
private:
	ofstream m_outStream;
	bool	 m_initialised;
};