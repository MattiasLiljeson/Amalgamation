#pragma once

#include <Component.h>
#include <ComponentFactory.h>
#include <Globals.h>
#include <string>

struct LevelPieceFileData;

enum EndPieceMode
{
	ENDPIECEMODE_OPENED,
	ENDPIECEMODE_CLOSED
};

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
	
	LevelPieceFileData* getEndPlugFileData(EndPieceMode p_mode) const;

	int		getBranchCount() const;
	bool	doRandomStartRotation() const;
	int		getLevelSize(int p_playerCount) const;
private:
	void addWeightData(LevelPieceFileData* p_fromFileData);
	void setLevelSize(string p_fromData);

private:
	vector<LevelPieceFileData*> m_fileData;
	vector<int>					m_weightData;
	LevelPieceFileData*			m_endPlugClosed;
	LevelPieceFileData*			m_endPlugOpened;

	int		m_levelSize[MAXPLAYERS];

	int		m_branchCount;
	bool	m_randomStartRotation;
	int		m_startPieceId;

	static ComponentRegister<LevelInfo> s_reg;
};