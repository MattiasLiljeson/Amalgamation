#pragma once

#include <exception>
#include <string>
#include "ToString.h"

using namespace std;

// =======================================================================================
//                                      MeshLoadException
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # MeshLoadException
/// Detailed description.....
/// Created on: 8-1-2013 
///---------------------------------------------------------------------------------------

class MeshLoadException : public exception
{
public:
	MeshLoadException(const string &p_msg)
	{
		m_msg = "\nMesh load error: ";
		m_msg.append(p_msg);
		m_msg+="\n";
	}
	virtual ~MeshLoadException(){}
	///-----------------------------------------------------------------------------------
	/// Retrieve the error message
	/// \return const char*
	///-----------------------------------------------------------------------------------
	virtual const char* what() const throw()
	{
		return m_msg.c_str();
	}
protected:
private:
	string m_msg;
};