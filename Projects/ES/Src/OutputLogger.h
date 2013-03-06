#pragma once

#include "EntitySystem.h"
#include <fstream>
using namespace std;

enum WriteType
{
	WRITETYPE_INFO,
	WRITETYPE_WARNING,
	WRITETYPE_ERROR
};

// =======================================================================================
//                                      OutputLogger
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # OutputLogger
/// Detailed description.....
/// Created on: 6-3-2013 
///---------------------------------------------------------------------------------------

class OutputLogger : public EntitySystem
{
public:
	OutputLogger(const char* p_fileName);
	~OutputLogger();

	void write(const char* p_msg, WriteType p_type = WRITETYPE_INFO);

	virtual bool checkProcessing();

protected:
private:
	ofstream m_outStream;
};