// =======================================================================================
//                                      ToString
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Useful ToString-function
///        
/// # ToString
/// Detailed description.....
/// Created on: 28-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#define Stringify(x) #x

#include <sstream>
using namespace std;

template <class T>
std::string toString (const T& val)
{
	// Convert input value to string
	// using stringstream
	stringstream strStream;
	strStream << val;
	return strStream.str();
}