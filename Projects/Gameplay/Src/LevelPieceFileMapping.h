#pragma once

#include <vector>
#include <string>
#include <Globals.h>
using namespace std;
// =======================================================================================
//                                      LevelPieceModelMapping
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LevelPieceModelMapping
/// Detailed description.....
/// Created on: 18-1-2013 
///---------------------------------------------------------------------------------------

class LevelPieceFileMapping
{
public:
	LevelPieceFileMapping();
	virtual ~LevelPieceFileMapping();

	const string&	getAssemblageFileName(unsigned int p_id) const;
	const string&	getClientAssemblageFileName(unsigned int p_id) const;
	const string&	getModelFileName(unsigned int p_id) const;
	int				getModelFileCount() const;
	int				getRandomPieceId() const;
	int				getEndPlugId() const;

private:
	vector<string> m_fileNames;
	vector<string> m_asdNames;
	vector<string> m_clientAsdNames;
	string m_unknown;
};