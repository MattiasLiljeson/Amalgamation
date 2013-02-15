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
private:
	vector<LevelPieceFileData*> m_fileData;
	int		m_branchCount;
	bool	m_randomStartRotation;
	
	static ComponentRegister<LevelInfo> s_reg;
};