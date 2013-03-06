#pragma once

#include <EntitySystem.h>
#include "StaticProp.h"

class LevelInfo;
struct LevelPieceFileData;

// =======================================================================================
//                                      LevelInfoLoader
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LevelInfoLoader
/// Detailed description.....
/// Created on: 18-2-2013 
///---------------------------------------------------------------------------------------

class LevelInfoLoader : public EntitySystem
{
public:
	LevelInfoLoader();
	virtual ~LevelInfoLoader();

	virtual void initialize();

	virtual void removed( Entity* p_entity );

	LevelPieceFileData* getFileData(int p_id, StaticPropType p_type) const;
protected:
private:
	LevelInfo* m_levelInfo;
};