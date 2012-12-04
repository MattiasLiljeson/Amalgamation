// =======================================================================================
//                                      PrintPositionsSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief description...
///        
/// # PrintPositionsSystem
/// Detailed description...
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <iostream>

#include <SystemType.h>
#include <EntitySystem.h>

// Components:
#include "Transform.h"

using namespace std;

class PrintPositionsSystem: public EntitySystem
{
public:
	PrintPositionsSystem();
	~PrintPositionsSystem();

	virtual void begin();

	virtual void end();

	virtual void processEntities( const vector<Entity*>& p_entities );

	virtual void initialize();

private:


};