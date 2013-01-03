#pragma once

#include <EntitySystem.h>

// =======================================================================================
//                                      LevelGenSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LevelGenSystem
/// Detailed description.....
/// Created on: 3-1-2013 
///---------------------------------------------------------------------------------------

class LevelGenSystem : public EntitySystem
{
public:
	LevelGenSystem();
	virtual ~LevelGenSystem();

	void initialize();
protected:

private:
	void generateLevelPieces(int p_depth);

};
