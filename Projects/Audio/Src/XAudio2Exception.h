#pragma once

#include <exception>
#include <string>
#include <Strsafe.h>
#include <ToString.h>

using namespace std;

// =======================================================================================
//                                      XAudio2Exception
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the possible exceptions sent from XAudio2
///        
/// # XAudio2Exception
/// Detailed description.....
/// Created on: 6-12-2012 
///---------------------------------------------------------------------------------------

class XAudio2Exception : public exception
{
public:
	XAudio2Exception( HRESULT p_hresult, const string &p_file, const string &p_func, 
		int p_line)
	{
		LPSTR errTxt = NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, p_hresult, MAKELANGID(LANG_NEUTRAL, 
			SUBLANG_DEFAULT), (LPSTR)&errTxt, 0, NULL);
		if (errTxt!=NULL)
			SetMsg(toString(errTxt),p_file,p_func,p_line);
		else
			SetMsg("Unknown HRESULT error! \n",p_file,p_func,p_line);
	}

	virtual const char* what() const throw()
	{
		return m_msg.c_str();
	}

private:
	///-----------------------------------------------------------------------------------
	/// Sets the layout and contents of the error message
	/// \param p_msg
	/// \param p_file
	/// \param p_func
	/// \param p_line
	/// \returns void
	///-----------------------------------------------------------------------------------
	void SetMsg(const string &p_msg,const string &p_file,const string &p_func,int p_line)
	{
		m_msg = "\nXAudio2 Error: ";
		m_msg.append(p_msg);
		m_msg.append(" > ");
		m_msg.append(p_file);
		m_msg.append(" : ");
		m_msg.append(p_func);
		m_msg.append(" line ");
		m_msg.append(toString(p_line));
		m_msg.append("\n\n");
	}

	string m_msg;
};