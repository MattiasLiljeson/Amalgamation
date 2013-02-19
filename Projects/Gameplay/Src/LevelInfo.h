#pragma once

#include <Component.h>
#include <ComponentFactory.h>

struct LevelPieceFileData;
// =======================================================================================
//                                      LevelInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LevelInfo
/// Detailed description.....
/// Created on: 15-2-2013 
///---------------------------------------------------------------------------------------

class LevelInfo : public Component
{
public:
	LevelInfo();
	~LevelInfo();

	void init( vector<ComponentData> p_initData );
	const vector<LevelPieceFileData*>& getFileData() const;
	LevelPieceFileData* getRandomFileData() const; // NOTE: Uses weight
	LevelPieceFileData* getStartFileData() const;
	LevelPieceFileData* getFileDataFromId(int p_id) const;
	int		getBranchCount() const;
	bool	doRandomStartRotation() const;

private:
	void addWeightData(LevelPieceFileData* p_fromFileData);

private:
	vector<LevelPieceFileData*> m_fileData;
	vector<int>					m_weightData;
	int		m_branchCount;
	bool	m_randomStartRotation;
	int		m_startPieceId;
	string	m_plugPiece;

	static ComponentRegister<LevelInfo> s_reg;
};