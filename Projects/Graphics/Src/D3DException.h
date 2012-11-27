// =======================================================================================
//                                      D3DException
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # D3DException
/// Detailed description.....
/// Created on: 27-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <exception>
#include <string>
using namespace std;

class D3DException : public exception
{
public:
	D3DException(const string &p_msg)
	{
		m_msg = p_msg;
	}
	virtual ~D3DException(){}
	virtual const char* what() const throw()
	{
		return string("D3D-Error: ").append(m_msg).c_str();
	}
private:
	string m_msg;
};