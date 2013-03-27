#pragma once
#include <EntitySystem.h>
#include <Globals.h>
#include <InputHelper.h>
class Control;
class InputBackendSystem;

// =======================================================================================
// InputActionsBackendSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief A system to use to get certain input states as an action instead of a key press
/// or other input methods.
///        
/// # InputActionsBackendSystem
/// Detailed description...
/// Created on: 11-2-2013 
///---------------------------------------------------------------------------------------
class InputActionsBackendSystem: public EntitySystem
{
public:
	enum Actions
	{
		Actions_NON_EXISTING = -1,
		
		Actions_ACTIVATE_SLOT_1,
		Actions_ACTIVATE_SLOT_2,
		Actions_ACTIVATE_SLOT_3,
		Actions_ACTIVATE_SLOT_4,
		Actions_ACTIVATE_PREV_SLOT,
		Actions_ACTIVATE_NEXT_SLOT,
		Actions_ACTIVATE_MODULE,
		Actions_ROTATE_MODULE_LEFT,
		Actions_ROTATE_MODULE_RIGHT,
		Actions_ROTATE90_MODULE_LEFT,
		Actions_ROTATE90_MODULE_RIGHT,
		Actions_TOGGLE_DESIRED_SLOT,
		Actions_STRAFE_LEFT,
		Actions_STRAFE_RIGHT,
		Actions_STRAFE_UP,
		Actions_STRAFE_DOWN,
		Actions_TURN_LEFT,
		Actions_TURN_RIGHT,
		Actions_TURN_UP,
		Actions_TURN_DOWN,
		Actions_EDIT_MODE_LEFT,
		Actions_EDIT_MODE_RIGHT,
		Actions_EDIT_MODE_UP,
		Actions_EDIT_MODE_DOWN,
		Actions_EDIT_MODE_ACTIVATE_ROTATION,
		Actions_CURSOR_LEFT,
		Actions_CURSOR_RIGHT,
		Actions_CURSOR_UP,
		Actions_CURSOR_DOWN,
		Actions_CURSOR_PRIMARY,
		Actions_CURSOR_SECONDARY,
		Actions_ROLL_LEFT,
		Actions_ROLL_RIGHT,
		Actions_THRUST_FORWARD,
		Actions_TRIGGER_EDIT_MODE,
		Actions_TOGGLE_EDIT_MODE,
		Actions_MOVE_MODULE,
		Actions_DROP_MODULE,
		Actions_MENU_RIGHT,
		Actions_MENU_LEFT,
		Actions_MENU_ACTIVATE_ROTATION,
		Actions_SHOW_SCORE,
		Actions_GAME_BACK,

		Actions_CNT //End Actions enum
	};

	InputActionsBackendSystem( string p_path = SETTINGSPATH, string p_file = "setting.input");
	~InputActionsBackendSystem();
	void initialize();
	void process();

	double getDeltaByAction(Actions p_action);
	double getStatusByAction(Actions p_action);
	Control* getControlByAction(Actions p_action, int p_index);
	Control* findControlOfDeviceByAction(Actions p_action, 
		InputHelper::InputDeviceTypes p_deviceType);
	void setMouseSensitivity(float p_sensitivity);
	const bool gamepadUsedLast();

private:
	void readControlFromString(string p_key, Control** p_control);
	void initCursor();
	void initControlMap();

private:
	InputBackendSystem* m_inputBackend;
	vector<Control*> m_inputControls[Actions_CNT];
	map<string, int> m_actionMap;
	string m_path;
	string m_file;

	enum Device { Device_NONE = -1, Device_MOUSE, Device_CONTROLLER, Device_CNT };

	double m_sensitivities[Device_CNT];
	double m_cursorSensitivities[Device_CNT];
	bool m_gamepadUsedLast; ///< True if last input was gamepad
};