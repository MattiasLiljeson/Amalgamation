#include "InputActionsBackendSystem.h"
#include <Control.h>
#include <fstream>
#include <DebugUtil.h>
#include "InputBackendSystem.h"

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
	m_inputBackend = static_cast<InputBackendSystem*>(
		m_world->getSystem( SystemType::InputBackendSystem) );
	ifstream file;
	file.open(m_inputIniFile.c_str());
	string token = "";
	string keyboardString = "";
	string xboxString = "";
	int currentAction = 0;
	if(file.is_open())
	{
		getline(file, token); // discard the first line which is a comment.
		while(!file.eof() && currentAction < (int)Actions_CNT)
		{
			file >> token; // discard the first column which is the activation enum
			file >> keyboardString;
			readControlFromString(keyboardString, &m_mappedKeyboardAndMouseInputControls[currentAction]);
			file >> xboxString;
			readControlFromString(xboxString, &m_mappedGamepadInputControls[currentAction]);
			currentAction += 1;
		}
	}
	else
	{
		DEBUGWARNING(( string("WARNING: File: " + m_inputIniFile +
			" could not be opened!").c_str() ));
	}
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
	if(m_mappedGamepadInputControls[p_action])
		delta += m_mappedGamepadInputControls[p_action]->getDelta();
	if(m_mappedKeyboardAndMouseInputControls[p_action])
		delta += m_mappedKeyboardAndMouseInputControls[p_action]->getDelta();
	if(delta > 1.0)
		delta = 1.0;
	return delta;
}

double InputActionsBackendSystem::getStatusByAction( Actions p_action )
{
	double status = 0.0;
	if(m_mappedGamepadInputControls[p_action])
		status += m_mappedGamepadInputControls[p_action]->getStatus();
	if(m_mappedKeyboardAndMouseInputControls[p_action])
		status += m_mappedKeyboardAndMouseInputControls[p_action]->getStatus();
	if(status > 1.0)
		status = 1.0;
	return status;
}
