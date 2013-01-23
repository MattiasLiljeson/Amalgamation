#pragma once
#include <exception>
#include <string>
#include <ToString.h>

using namespace std;

// =======================================================================================
//                                      AssemblageException
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # AssemblageException
/// Detailed description.....
/// Created on: 22-1-2013 
///---------------------------------------------------------------------------------------

class AssemblageException : public exception
{
public:
	AssemblageException(string& p_msg,const string &p_file,const string &p_func,int p_line)
	{
		m_msg = "\nAssemblage error: ";
		m_msg.append(p_msg);
		m_msg.append(" > ");
		m_msg.append(p_file);
		m_msg.append(" : ");
		m_msg.append(p_func);
		m_msg.append(" line ");
		m_msg.append(toString(p_line));
		m_msg.append("\n\n");
	}
	virtual ~AssemblageException() {}

	virtual const char* what() const throw()
	{
		return m_msg.c_str();
	}
protected:
private:
	string m_msg;
};