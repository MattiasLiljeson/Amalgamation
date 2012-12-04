// =======================================================================================
//                                      InputSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # InputSystem
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <EntitySystem.h>
#include <InputControlFactory.h>
#include <InputManager.h>

class inputControlFactory;
class InputManager;

class InputSystem : public EntitySystem
{
public:
	InputSystem(void);
	~InputSystem(void);

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );

private:
	InputManager* m_manager;
	vector<int> m_controlIdxs;
};

