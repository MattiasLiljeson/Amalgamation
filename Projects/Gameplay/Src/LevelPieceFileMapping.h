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

// LevelPieceFileData Not Yet Implemented!
struct LevelPieceFileData
{
	int		id;
	string	modelFileName;
	string	assemblageName;
	string	assemblageFileName;
	int		weight;
};

class LevelPieceFileMapping
{
public:
	LevelPieceFileMapping();
	virtual ~LevelPieceFileMapping();

	void resetToDefault();

	const string&	getAssemblageFileName(unsigned int p_id) const;
	const string&	getClientAssemblageFileName(unsigned int p_id) const;
	const string&	getModelFileName(unsigned int p_id) const;
	int				getModelFileCount() const;
	int				getRandomPieceId() const;
	int				getEndPlugId() const;

	const LevelPieceFileData& getFileData(int p_id);

	int		getWeight(int p_id);
	void	setWeight(int p_id, int p_newWeight);
private:
	vector<LevelPieceFileData> m_fileData;
	vector<int> m_weightData;

	vector<string> m_fileNames;
	vector<string> m_asdNames;
	vector<string> m_clientAsdNames;
	string m_unknown;
};