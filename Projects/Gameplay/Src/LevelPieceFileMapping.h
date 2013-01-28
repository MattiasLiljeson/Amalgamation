#pragma once

#include <vector>
#include <string>
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

	const string&	getModelFileName(unsigned int p_id) const;
	int				getModelFileCount() const;
	int				getRandomPieceId() const;
	int				getEndPlugId() const;
protected:
private:
	vector<string> m_fileNames;
};