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


#include "CameraInfo.h"
#include "Input.h"
#include "Transform.h"
#include <EntitySystem.h>
#include <InputControlFactory.h>
#include <InputManager.h>
#include <MessageLoopFetcher.h>
#include <XInputFetcher.h>
#include <string>
#include <map>

class InputHelper;
class inputControlFactory;
class InputManager;

using namespace std;

class InputSystem : public EntitySystem
{
public:
	InputSystem();
	~InputSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );

private:
	InputManager* m_inputManager;
	map<string, int> m_controlIdxs;
};

