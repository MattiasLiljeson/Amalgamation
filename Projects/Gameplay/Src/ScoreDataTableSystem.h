#pragma once

#include <EntitySystem.h>
#include <Rocket/Controls/DataSource.h>

// =======================================================================================
//                                      ScoreDataTableSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ScoreDataTableSystem
/// Detailed description.....
/// Created on: 23-1-2013 
///---------------------------------------------------------------------------------------

class ScoreDataTableSystem : public EntitySystem, public Rocket::Controls::DataSource
{
public:
	ScoreDataTableSystem();

	void initialize();

	virtual ~ScoreDataTableSystem();
protected:
private:
};