#pragma once

#include <vector>
#include <string>
#include <Globals.h>
using namespace std;


// =======================================================================================
//                                      LevelPieceFileData
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LevelPieceFileData
/// Detailed description.....
/// Created on: 18-1-2013 
///---------------------------------------------------------------------------------------

struct LevelPieceFileData
{
	int		id;
	string	modelFileName;
	string	assemblageName;
	string	assemblageFileName;
	int		weight;

	LevelPieceFileData()
	{
		id					= -1;
		modelFileName		= "(unknown)";
		assemblageName		= "(unknown)";
		assemblageFileName	= "(unknown)";
		weight				= 0;
	}
};
