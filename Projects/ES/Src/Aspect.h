#pragma once
// =======================================================================================
//                                      Aspect
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	NOT IMPLEMENTED!
///        
/// # Aspect
/// This is a stub class for Aspects. The functions below are not implemented.
/// Created on: 27-11-2012 
///---------------------------------------------------------------------------------------

#include <bitset>
#include <stdarg.h>

using namespace std;

class Aspect
{
public:
	///-----------------------------------------------------------------------------------
	/// Desc
	/// \param int p_numComponents Number of component types passed.
	/// \param ... Component types that are seen as an aspect by the System.
	/// \returns
	///-----------------------------------------------------------------------------------
	Aspect( int p_numComponents, ... );

	~Aspect(void);

	bitset getAllSet();
	bitset getExlusionSet();
	bitset getOneSet();

	Aspect all( int p_numComponents, ... );
	Aspect exclude( int p_numComponents, ... );
	Aspect one( int p_numComponents, ... );


private:
	bitset allSet;
	bitset exclusionSet;
	bitset oneSet;
};

