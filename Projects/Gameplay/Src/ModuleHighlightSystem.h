#pragma once
#include <EntitySystem.h>
// =======================================================================================
// ModuleHighlightSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # ModuleHighlightSystem
/// Detailed description...
/// Created on: 25-2-2013 
///---------------------------------------------------------------------------------------
class ModuleHighlightSystem: public EntitySystem
{
private:
public:
	ModuleHighlightSystem();
	virtual void processEntities( const vector<Entity*>& p_entities ) final;

};