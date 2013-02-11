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
	InputBackendSystem* inputBackend = static_cast<InputBackendSystem*>(
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
			file >> xboxString;
			m_mappedKeyboardAndMouseInputControls[currentAction] =
				inputBackend->getInputControl( keyboardString );
			m_mappedGamepadInputControls[currentAction] =
				inputBackend->getInputControl( xboxString );
			currentAction += 1;
		}
	}
	else
	{
		DEBUGWARNING(( string("WARNING: File: " + m_inputIniFile +
			" could not be opened!").c_str() ));
	}
}

double InputActionsBackendSystem::getDeltaByAction( Actions p_action )
{
	return 0.0;
}