#pragma once

#include <exception>
#include <string>
#include <Strsafe.h>
#include "ToString.h"

using namespace std;

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

class D3DException : public exception
{
public:

	///-----------------------------------------------------------------------------------
	/// Constructor for throwing HRESULTs
	/// \param p_hresult
	/// \param p_file
	/// \param p_func
	/// \param p_line
	/// \return 
	///-----------------------------------------------------------------------------------
	D3DException(HRESULT p_hresult,const string &p_file,const string &p_func,int p_line)
	{
		LPSTR errTxt = NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, p_hresult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&errTxt, 0, NULL);
		if (errTxt!=NULL)
			SetMsg(toString(errTxt),p_file,p_func,p_line);
		else
			SetMsg("Unknown HRESULT error! \n",p_file,p_func,p_line);
	}

	///-----------------------------------------------------------------------------------
	/// Constructor for throwing D3D error blobs
	/// \param p_errorBlob
	/// \param p_file
	/// \param p_func
	/// \param p_line
	/// \return 
	///-----------------------------------------------------------------------------------
	D3DException(ID3DBlob* p_errorBlob,const string &p_file,const string &p_func,int p_line)
	{
		char msg[1000];
		strcpy_s(msg, sizeof(msg), (char*)p_errorBlob->GetBufferPointer());
		SetMsg(toString(msg),p_file,p_func,p_line);
	}

	///-----------------------------------------------------------------------------------
	/// Constructor for throwing raw strings
	/// \param p_msg
	/// \param p_file
	/// \param p_func
	/// \param p_line
	/// \return 
	///-----------------------------------------------------------------------------------
	D3DException(const string &p_msg,const string &p_file,const string &p_func,int p_line)
	{
		SetMsg(p_msg,p_file,p_func,p_line);
	}
	virtual ~D3DException(){}
	///-----------------------------------------------------------------------------------
	/// Retrieve the error message
	/// \return const char*
	///-----------------------------------------------------------------------------------
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
	/// \return void
	///-----------------------------------------------------------------------------------
	void SetMsg(const string &p_msg,const string &p_file,const string &p_func,int p_line)
	{
		m_msg = "\nD3D Error: ";
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