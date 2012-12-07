// =======================================================================================
//                                    Window Exception
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # Exceptions
/// Detailed description.....
/// Created on: 27-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <exception>

using namespace std;

class WindowException : public exception
{
public:
	WindowException(){}
	virtual ~WindowException(){}
	virtual const char* what() const throw()
	{
		return "Failed to init window.";
	}

protected:
private:
};