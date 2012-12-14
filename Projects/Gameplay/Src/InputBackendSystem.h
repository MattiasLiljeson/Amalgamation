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
#include "GraphicsBackendSystem.h"
#include "Input.h"
#include "Transform.h"
#include <DirectInputFetcher.h>
#include <EntitySystem.h>
#include <IMouseKeyboardFetcher.h>
#include <InputControlFactory.h>
#include <InputManager.h>
#include <MessageLoopFetcher.h>
#include <XInputFetcher.h>
#include <Windows.h>
#include <map>
#include <string>

class InputHelper;
class inputControlFactory;
class InputManager;

using namespace std;

class InputBackendSystem : public EntitySystem
{
public:
	InputBackendSystem( HINSTANCE p_hInstance, GraphicsBackendSystem* p_graphicsBackend );
	~InputBackendSystem();

	virtual void initialize();
	void process();

	Control* getInputControl(const string& p_name);

private:
	GraphicsBackendSystem* m_graphicsBackend;
	HINSTANCE m_hInstance;
	InputManager* m_inputManager;
	map<string, int> m_controlIdxs;
};

