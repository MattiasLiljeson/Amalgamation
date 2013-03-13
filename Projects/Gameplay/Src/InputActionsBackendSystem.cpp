#include "InputActionsBackendSystem.h"
#include <Control.h>
#include <fstream>
#include <DebugUtil.h>
#include "InputBackendSystem.h"
#include <Cursor.h>

InputActionsBackendSystem::InputActionsBackendSystem( string p_inputIniFile )
	: EntitySystem( SystemType::InputActionsBackendSystem )
{
	m_inputIniFile = p_inputIniFile;
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
	file.open(m_inputIniFile.c_str());
	string line = "";
	string token = "";
	//int currentAction = 0;
	if(file.is_open())
	{
		getline(file, line); // discard the first line which is a comment.
		while( file.good() )
		{
			getline(file, line);
			stringstream ss(line);
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
			}
		}
	}
	else
	{
		DEBUGWARNING(( string("WARNING: File: " + m_inputIniFile +
			" could not be opened!").c_str() ));
	}
	initCursor();
}
void InputActionsBackendSystem::readControlFromString(string p_key, Control** p_control)
{
	if(p_key != "NULL")
	{
		*p_control =
			m_inputBackend->getInputControl( p_key );
	}
	else
	{
		*p_control = NULL;
	}
}

double InputActionsBackendSystem::getDeltaByAction( Actions p_action )
{
	double delta = 0.0;
	for(unsigned int i=0; i<m_inputControls[p_action].size(); i++)
	{
		delta += m_inputControls[p_action][i]->getDelta();
	}
	return delta;
}

double InputActionsBackendSystem::getStatusByAction( Actions p_action )
{
	double status = 0.0;
	for(unsigned int i=0; i<m_inputControls[p_action].size(); i++)
	{
		status += m_inputControls[p_action][i]->getStatus();
	}
	return status;
}

Control* InputActionsBackendSystem::getControlByAction( Actions p_action, int p_index )
{
	if(m_inputControls[p_action].size() >= p_index)
		return m_inputControls[p_action][p_index];
	return NULL;
}

void InputActionsBackendSystem::initCursor()
{
	Cursor* cursor = m_inputBackend->getCursor();
	cursor->addControlSet(
		60.0, 60.0, false,
		getControlByAction( InputActionsBackendSystem::Actions_CURSOR_LEFT, 0 ),
		getControlByAction( InputActionsBackendSystem::Actions_CURSOR_RIGHT, 0 ),
		getControlByAction( InputActionsBackendSystem::Actions_CURSOR_UP, 0 ),
		getControlByAction( InputActionsBackendSystem::Actions_CURSOR_DOWN, 0 ),
		getControlByAction( InputActionsBackendSystem::Actions_CURSOR_PRIMARY, 0 ),
		getControlByAction( InputActionsBackendSystem::Actions_CURSOR_SECONDARY, 0 ) );
	cursor->addControlSet(
		1000.0, 1000.0, true,
		getControlByAction( InputActionsBackendSystem::Actions_CURSOR_LEFT, 1 ),
		getControlByAction( InputActionsBackendSystem::Actions_CURSOR_RIGHT, 1 ),
		getControlByAction( InputActionsBackendSystem::Actions_CURSOR_UP, 1 ),
		getControlByAction( InputActionsBackendSystem::Actions_CURSOR_DOWN, 1 ),
		getControlByAction( InputActionsBackendSystem::Actions_CURSOR_PRIMARY, 1 ),
		getControlByAction( InputActionsBackendSystem::Actions_CURSOR_SECONDARY, 1 ) );

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
