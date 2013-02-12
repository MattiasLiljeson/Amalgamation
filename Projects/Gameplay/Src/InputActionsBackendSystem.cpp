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
	string line = "";
	string token = "";
	int currentAction = 0;
	if(file.is_open())
	{
		getline(file, line); // discard the first line which is a comment.
		while(!file.eof() && currentAction < (int)Actions_CNT)
		{
			getline(file, line);
			stringstream ss(line);
			ss >> token; // discard the first column which is the activation enum
			while(!ss.eof())
			{
				ss >> token;
				m_inputControls[currentAction].push_back(NULL);
				readControlFromString(token, &m_inputControls[currentAction].back());
			}
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
