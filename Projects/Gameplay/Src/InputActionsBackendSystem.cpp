#include "InputActionsBackendSystem.h"
#include <Control.h>
#include <fstream>
#include <DebugUtil.h>
#include "InputBackendSystem.h"
#include <Cursor.h>

InputActionsBackendSystem::InputActionsBackendSystem( string p_path, string p_file )
	: EntitySystem( SystemType::InputActionsBackendSystem )
{
	m_path = p_path;
	m_file = p_file;

	m_sensitivities[Device_MOUSE]			= 1.0;
	m_cursorSensitivities[Device_MOUSE]		= 1.0;
	m_sensitivities[Device_CONTROLLER]		= 1.0;
	m_cursorSensitivities[Device_CONTROLLER]	= 1.0;
	m_gamepadUsedLast=false;
}

InputActionsBackendSystem::~InputActionsBackendSystem()
{
}

void InputActionsBackendSystem::initialize()
{
	initControlMap();

	m_inputBackend = static_cast<InputBackendSystem*>(
		m_world->getSystem( SystemType::InputBackendSystem) );
	ifstream file;
	string filePath = m_path + m_file;
	file.open( filePath.c_str() );

	// Load std settings if given settings file isn't found
	if( !file.is_open() ) {
		file.close();
		filePath = m_path + "stdSettings.input";
		file.open( filePath.c_str() );
	}

	if(file.is_open())
	{
		string line = "";
		string token = "";
		getline( file, line ); // discard the first line which is a comment.
		while( file.good() )
		{
			getline( file, line );
			stringstream ss( line );
			ss >> token; // discard the first column which is the activation enum

			if( m_actionMap.count( token ) )
			{
				int action = m_actionMap[token];
				while( !ss.eof() )
				{
					ss >> token;
					m_inputControls[action].push_back(NULL);
					readControlFromString( token, &m_inputControls[action].back() );
				}
			} else if( token == "controllerSensitivity" ) {
				ss >> m_sensitivities[Device_CONTROLLER];
			} else if( token == "mouseSensitivity" ) {
				ss >> m_sensitivities[Device_MOUSE];
			}else if( token == "menuControllerSensitivity" ) {
				ss >> m_cursorSensitivities[Device_CONTROLLER];
			} else if( token == "menuMouseSensitivity" ) {
				ss >> m_cursorSensitivities[Device_MOUSE];
			}
		}
	}
	else
	{
		DEBUGWARNING(( string( "WARNING: File: " + m_path + m_file +
			" could not be opened!" ).c_str() ));
	}

	m_inputBackend->setControllerSensitivity( m_sensitivities[Device_CONTROLLER] );
	m_inputBackend->setMouseSensitivity( m_sensitivities[Device_MOUSE] );
	initCursor();
}
void InputActionsBackendSystem::readControlFromString(string p_key, Control** p_control)
{
	if(p_key != "NULL") {
		*p_control = m_inputBackend->getInputControl( p_key );
	} else {
		*p_control = NULL;
	}
}

double InputActionsBackendSystem::getDeltaByAction( Actions p_action )
{
	double delta = 0.0;
	for(unsigned int i=0; i<m_inputControls[p_action].size(); i++)
	{
		Control* currentControl = m_inputControls[p_action][i];
		float currentDelta = currentControl->getDelta();
		InputHelper::InputDeviceTypes deviceT = currentControl->getDeviceType();
		if (deviceT == InputHelper::InputDeviceTypes_XINPUT_DIGITAL ||
			deviceT == InputHelper::InputDeviceTypes_XINPUT_ANALOG) 
		{
			if (currentDelta>0.0f) m_gamepadUsedLast=true;
		}
		else
		{
			if (currentDelta>0.1f && 
				deviceT != InputHelper::InputDeviceTypes_MOUSE_AXIS) 
				m_gamepadUsedLast=false;
		}

		delta += currentDelta;
	}
	return delta;
}

double InputActionsBackendSystem::getStatusByAction( Actions p_action )
{
	double status = 0.0;
	for(unsigned int i=0; i<m_inputControls[p_action].size(); i++)
	{
		Control* currentControl = m_inputControls[p_action][i];
		float currentStatus = currentControl->getStatus();
		InputHelper::InputDeviceTypes deviceT = currentControl->getDeviceType();
		if (deviceT == InputHelper::InputDeviceTypes_XINPUT_DIGITAL ||
			deviceT == InputHelper::InputDeviceTypes_XINPUT_ANALOG) 
		{
			if (currentStatus>0.0f) m_gamepadUsedLast=true;
		}
		else
		{
			if (currentStatus>0.1f && 
				deviceT != InputHelper::InputDeviceTypes_MOUSE_AXIS) 
				m_gamepadUsedLast=false;
		}

		status += currentStatus;
	}
	return status;
}

Control* InputActionsBackendSystem::getControlByAction( Actions p_action, int p_index )
{
	if(m_inputControls[p_action].size() >= p_index)
		return m_inputControls[p_action][p_index];
	return NULL;
}


Control* InputActionsBackendSystem::findControlOfDeviceByAction( Actions p_action,
																 InputHelper::InputDeviceTypes p_deviceType )
{
	for (int i=0;i<m_inputControls[p_action].size();i++)
	{
		Control* ctrl = m_inputControls[p_action][i];
		if (ctrl->getDeviceType()==p_deviceType)
			return ctrl;
	}
	return NULL;
}


void InputActionsBackendSystem::initCursor()
{
	Cursor* cursor = m_inputBackend->getCursor();

	for( int i=0; i<Device_CNT; i++ ) {
		cursor->addControlSet(
			m_cursorSensitivities[i], m_cursorSensitivities[i],
			getControlByAction( InputActionsBackendSystem::Actions_CURSOR_LEFT, i ),
			getControlByAction( InputActionsBackendSystem::Actions_CURSOR_RIGHT, i ),
			getControlByAction( InputActionsBackendSystem::Actions_CURSOR_UP, i ),
			getControlByAction( InputActionsBackendSystem::Actions_CURSOR_DOWN, i ),
			getControlByAction( InputActionsBackendSystem::Actions_CURSOR_PRIMARY, i ),
			getControlByAction( InputActionsBackendSystem::Actions_CURSOR_SECONDARY, i )
			);
	}
}

void InputActionsBackendSystem::initControlMap()
{
	m_actionMap["Actions_ACTIVATE_SLOT_1"]				= Actions_ACTIVATE_SLOT_1;
	m_actionMap["Actions_ACTIVATE_SLOT_2"]				= Actions_ACTIVATE_SLOT_2;
	m_actionMap["Actions_ACTIVATE_SLOT_3"]				= Actions_ACTIVATE_SLOT_3;
	m_actionMap["Actions_ACTIVATE_SLOT_4"]				= Actions_ACTIVATE_SLOT_4;
	m_actionMap["Actions_ACTIVATE_PREV_SLOT"]			= Actions_ACTIVATE_PREV_SLOT;
	m_actionMap["Actions_ACTIVATE_NEXT_SLOT"]			= Actions_ACTIVATE_NEXT_SLOT;
	m_actionMap["Actions_ACTIVATE_MODULE"]				= Actions_ACTIVATE_MODULE;
	m_actionMap["Actions_ROTATE_MODULE_LEFT"]			= Actions_ROTATE_MODULE_LEFT;
	m_actionMap["Actions_ROTATE_MODULE_RIGHT"]			= Actions_ROTATE_MODULE_RIGHT;
	m_actionMap["Actions_ROTATE90_MODULE_LEFT"]			= Actions_ROTATE90_MODULE_LEFT;
	m_actionMap["Actions_ROTATE90_MODULE_RIGHT"]		= Actions_ROTATE90_MODULE_RIGHT;
	m_actionMap["Actions_TOGGLE_DESIRED_SLOT"]			= Actions_TOGGLE_DESIRED_SLOT;
	m_actionMap["Actions_STRAFE_LEFT"]					= Actions_STRAFE_LEFT;
	m_actionMap["Actions_STRAFE_RIGHT"]					= Actions_STRAFE_RIGHT;
	m_actionMap["Actions_STRAFE_UP"]					= Actions_STRAFE_UP;
	m_actionMap["Actions_STRAFE_DOWN"]					= Actions_STRAFE_DOWN;
	m_actionMap["Actions_TURN_LEFT"]					= Actions_TURN_LEFT;
	m_actionMap["Actions_TURN_RIGHT"]					= Actions_TURN_RIGHT;
	m_actionMap["Actions_TURN_UP"]						= Actions_TURN_UP;
	m_actionMap["Actions_TURN_DOWN"]					= Actions_TURN_DOWN;
	m_actionMap["Actions_EDIT_MODE_LEFT"]				= Actions_EDIT_MODE_LEFT;
	m_actionMap["Actions_EDIT_MODE_RIGHT"]				= Actions_EDIT_MODE_RIGHT;
	m_actionMap["Actions_EDIT_MODE_UP"]					= Actions_EDIT_MODE_UP;
	m_actionMap["Actions_EDIT_MODE_DOWN"]				= Actions_EDIT_MODE_DOWN;
	m_actionMap["Actions_EDIT_MODE_ACTIVATE_ROTATION"]	= Actions_EDIT_MODE_ACTIVATE_ROTATION;
	m_actionMap["Actions_CURSOR_LEFT"]					= Actions_CURSOR_LEFT;
	m_actionMap["Actions_CURSOR_RIGHT"]					= Actions_CURSOR_RIGHT;
	m_actionMap["Actions_CURSOR_UP"]					= Actions_CURSOR_UP;
	m_actionMap["Actions_CURSOR_DOWN"]					= Actions_CURSOR_DOWN;
	m_actionMap["Actions_CURSOR_PRIMARY"]				= Actions_CURSOR_PRIMARY;
	m_actionMap["Actions_CURSOR_SECONDARY"]				= Actions_CURSOR_SECONDARY;
	m_actionMap["Actions_ROLL_LEFT"]					= Actions_ROLL_LEFT;
	m_actionMap["Actions_ROLL_RIGHT"]					= Actions_ROLL_RIGHT;
	m_actionMap["Actions_THRUST_FORWARD"]				= Actions_THRUST_FORWARD;
	m_actionMap["Actions_TRIGGER_EDIT_MODE"]			= Actions_TRIGGER_EDIT_MODE;
	m_actionMap["Actions_TOGGLE_EDIT_MODE"]				= Actions_TOGGLE_EDIT_MODE;
	m_actionMap["Actions_MOVE_MODULE"]					= Actions_MOVE_MODULE;
	m_actionMap["Actions_DROP_MODULE"]					= Actions_DROP_MODULE;
	m_actionMap["Actions_MENU_RIGHT"]					= Actions_MENU_RIGHT;
	m_actionMap["Actions_MENU_LEFT"]					= Actions_MENU_LEFT;
	m_actionMap["Actions_MENU_ACTIVATE_ROTATION"]		= Actions_MENU_ACTIVATE_ROTATION;
	m_actionMap["Actions_SHOW_SCORE"]					= Actions_SHOW_SCORE;
	m_actionMap["Actions_GAME_QUIT"]					= Actions_GAME_BACK;
}

const bool InputActionsBackendSystem::gamepadUsedLast()
{
	return m_gamepadUsedLast;
}
