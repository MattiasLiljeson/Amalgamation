#pragma once

#include "DebugUtil.h"
#include <string>
#include <fstream>
using namespace std;

// =======================================================================================
//                                      FileCheck
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Checks if file is ok
///        
/// # FileCheck
/// Detailed description.....
/// Created on: 8-1-2013 
///---------------------------------------------------------------------------------------

bool isFileOk(const string& filepath, 
			  string& p_outMsg,
			  const string &p_callerCodeFile,
			  const string &p_callerCodeFunc,
			  int p_callerCodeLine);