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
#include <InputType.h>
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
	Control* getControlIdxsByEnum( InputHelper::KEYBOARD_KEY p_key );
	Control* getControlIdxsByEnum( InputHelper::MOUSE_BTN p_btn );
	Control* getControlIdxsByEnum( InputHelper::MOUSE_AXIS p_axis );
	Control* getControlIdxsByEnum( InputHelper::XBOX360_CONTROLLER_DIGITAL p_digital );
	Control* getControlIdxsByEnum( InputHelper::XBOX360_CONTROLLER_ANALOG p_analog );
private:
	void saveControl( InputHelper::INPUT_DEVICE_TYPE p_deviceType, int p_controlType,
		Control* p_control, const string& p_name );
	vector<int>* vectorFromDeviceType( InputHelper::INPUT_DEVICE_TYPE p_deviceType );
	void expandIdxVectorIfNecessary( vector<int>* p_vec, int p_idx );

private:
	GraphicsBackendSystem* m_graphicsBackend;
	HINSTANCE m_hInstance;
	InputManager* m_inputManager;

	map<string, int> m_controlIdxs;
	vector<int> m_mouseMoveIdxsbyEnum;
	vector<int> m_mouseBtnIdxsbyEnum;
	vector<int> m_keyIdxsbyEnum;
	vector<int> m_xboxAnalogIdxsbyEnum;
	vector<int> m_xboxDigitalIdxsbyEnum;
};

