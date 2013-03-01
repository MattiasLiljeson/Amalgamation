#pragma once

#include <EntitySystem.h>

class GraphicsBackendSystem;
class InputBackendSystem;

// =======================================================================================
//                                      SpriteSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # SpriteSystem
/// Detailed description.....
/// Created on: 20-12-2012 
///---------------------------------------------------------------------------------------

class SpriteSystem : public EntitySystem
{
public:
	SpriteSystem();
	~SpriteSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );
private:
};

