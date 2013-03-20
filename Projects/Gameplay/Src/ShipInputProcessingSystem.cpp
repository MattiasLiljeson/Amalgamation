#include "ShipInputProcessingSystem.h"
#include "InputActionsBackendSystem.h"
#include "InputBackendSystem.h"
#include "Control.h"
#include <ValueClamp.h>
#include "AntTweakBarWrapper.h"
#include "HighlightSlotPacket.h"
#include "SimpleEventPacket.h"
#include "Cursor.h"
#include <Globals.h>
#include <OutputLogger.h>
#include <ToString.h>
#include <DebugUtil.h>
#include "TimerSystem.h"
#include <InputHelper.h>

const string ShipInputProcessingSystem::gamepadmap[] = {
	"^",
	"v",
	"<",
	">",
	"S",
	"[",
	"O",
	"O",
	"[L]",
	"[R]",
	"<p style='color:rgba(181,230,29,220);'>A</p>",
	"<p style='color:rgba(255,62,34,220);'>B</p>",
	"<p style='color:rgba(34,122,255,220);'>X</p>",
	"<p style='color:rgba(255,200,34,220);'>Y</p>"
};


const string ShipInputProcessingSystem::keymap[] = {
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"[",		
	"'",		
	";",		
	"*",
	"*",
	"*",
	"_",
	"*",
	"*",
	"*",
	"b",
	"d",
	"r",
	".",
	"*",
	"*",
	"*",
	"*",
	"*",
	"<",
	">",
	"<",
	"^",
	"v",
	">",
	"<",
	"^",
	"v",
	">",
	"f1",
	"f2",
	"f3",
	"f4",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
};

ShipInputProcessingSystem::ShipInputProcessingSystem(InputBackendSystem* p_inputBackend) :
										EntitySystem( SystemType::ShipInputProcessingSystem )
{
	m_angleInputMultiplier = 10.0f;
	m_controllerEpsilon = 0.15f;
	m_editSwitchTrigReleased = true;
	m_inputBackend = p_inputBackend;
	m_prevDeviceWasGamepad=false;
	m_hudMappingsInited=false;
}

void ShipInputProcessingSystem::initialize()
{
	m_actionBackend = static_cast<InputActionsBackendSystem*>(m_world->getSystem(
		SystemType::InputActionsBackendSystem));

	m_hudSystem = static_cast<HudSystem*>(m_world->getSystem(
		SystemType::HudSystem));

	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::INPUT,
		"ControllerEpsilon", TwType::TW_TYPE_FLOAT, getControllerEpsilonPointer(), 
		"min=0.0 max=1.0 step=0.05" );

	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::INPUT,
		"MouseSensitivity", TwType::TW_TYPE_FLOAT, &m_angleInputMultiplier, "");
}

void ShipInputProcessingSystem::process()
{
	if (!m_hudMappingsInited)
	{
		m_hudMappingsInited=true;
		updateHudKeymappings();
	}
	// This is read from the settings.input file naow. /ML
	//m_inputBackend->setMouseSensitivity(m_angleInputMultiplier);
	// Fetch the status of the various input methods.
	m_processedInput = readAllInput();

	if(static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
		checkTimeInterval(TimerIntervals::EverySecond))
	{
		if (m_actionBackend->gamepadUsedLast())
		{
			if (!m_prevDeviceWasGamepad)
			{
				m_prevDeviceWasGamepad=true;
				updateHudKeymappings(true);
			}
		}
		else
		{
			if (m_prevDeviceWasGamepad)
			{
				m_prevDeviceWasGamepad=false;
				updateHudKeymappings();
			}
		}
	}

	// Apply correction vectors to the analogue sticks.
	//m_processedInput.horizontalInput += static_cast<float>(m_leftStickCorrection[0]);
	//m_processedInput.verticalInput += static_cast<float>(m_leftStickCorrection[1]);

	// Apply a threshold value to eliminate some of the analogue stick noise.
// 	if( abs(m_processedInput.horizontalInput) < m_controllerEpsilon ) 
// 		m_processedInput.horizontalInput = 0;
// 
// 	if( abs(m_processedInput.verticalInput) < m_controllerEpsilon ) 
// 		m_processedInput.verticalInput = 0;
// 
// 	if( abs(m_processedInput.strafeHorizontalInput) < m_controllerEpsilon ) 
// 		m_processedInput.strafeHorizontalInput = 0;
// 
// 	if( abs(m_processedInput.strafeVerticalInput) < m_controllerEpsilon ) 
// 		m_processedInput.strafeVerticalInput = 0;


	// updateAntTweakBar(m_processedInput);
}

void ShipInputProcessingSystem::setCursorVisibility(bool p_show)
{
	if (p_show)
		m_inputBackend->getCursor()->show();
	else
		m_inputBackend->getCursor()->hide();
}


void ShipInputProcessingSystem::resetCursor()
{
	m_inputBackend->getCursor()->reset();
}


ShipInputProcessingSystem::ResultingInputForces& ShipInputProcessingSystem::getProcessedInput()
{
	return m_processedInput;
}

float* ShipInputProcessingSystem::getControllerEpsilonPointer()
{
	return &m_controllerEpsilon;
}

ShipInputProcessingSystem::RawInputForces ShipInputProcessingSystem::readAllInput()
{
	RawInputForces input;

	// rotate

	input.hPositive = m_actionBackend->getStatusByAction(
		InputActionsBackendSystem::Actions_TURN_RIGHT);
	saturate(input.hPositive); // ?

	input.hNegative = m_actionBackend->getStatusByAction(
		InputActionsBackendSystem::Actions_TURN_LEFT);
	saturate(input.hNegative); // ?

	input.vPositive = m_actionBackend->getStatusByAction(
		InputActionsBackendSystem::Actions_TURN_DOWN);
	saturate(input.vPositive); // ?

	input.vNegative = m_actionBackend->getStatusByAction(
		InputActionsBackendSystem::Actions_TURN_UP);
	saturate(input.vNegative); // ?

	// strafe

	input.shPositive = m_actionBackend->getStatusByAction(
		InputActionsBackendSystem::Actions_STRAFE_RIGHT);
	saturate(input.shPositive);

	input.shNegative = m_actionBackend->getStatusByAction(
		InputActionsBackendSystem::Actions_STRAFE_LEFT);
	saturate(input.shNegative);

	input.svPositive = m_actionBackend->getStatusByAction(
		InputActionsBackendSystem::Actions_STRAFE_UP);
	saturate(input.svPositive);

	input.svNegative = m_actionBackend->getStatusByAction(
		InputActionsBackendSystem::Actions_STRAFE_DOWN);
	saturate(input.svNegative);

	// edit mode rotate
	if(m_actionBackend->getStatusByAction(
		InputActionsBackendSystem::Actions_EDIT_MODE_ACTIVATE_ROTATION) > 0.0)
	{
		input.ehPositive = m_actionBackend->getStatusByAction(
			InputActionsBackendSystem::Actions_EDIT_MODE_RIGHT);
		saturate(input.ehPositive);

		input.ehNegative = m_actionBackend->getStatusByAction(
			InputActionsBackendSystem::Actions_EDIT_MODE_LEFT);
		saturate(input.ehNegative);

		input.evPositive = m_actionBackend->getStatusByAction(
			InputActionsBackendSystem::Actions_EDIT_MODE_UP);
		saturate(input.evPositive);

		input.evNegative = m_actionBackend->getStatusByAction(
			InputActionsBackendSystem::Actions_EDIT_MODE_DOWN);
		saturate(input.evNegative);
	}

	// roll

	input.rRight = m_actionBackend->getStatusByAction(
		InputActionsBackendSystem::Actions_ROLL_RIGHT);
	saturate(input.rRight);

	input.rLeft = m_actionBackend->getStatusByAction(
		InputActionsBackendSystem::Actions_ROLL_LEFT);
	saturate(input.rLeft);

	// thrust

	input.thrust = m_actionBackend->getStatusByAction(
		InputActionsBackendSystem::Actions_THRUST_FORWARD);
	saturate(input.thrust);

	// edit mode toggle
	double editModeToggle = m_actionBackend->getDeltaByAction(
		InputActionsBackendSystem::Actions_TOGGLE_EDIT_MODE);
	if(editModeToggle>0.0)
	{
		m_world->getOutputLogger()->write("Edit mode toggle\n");
		input.toggleEditMode = true;
	}


	// edit mode trigger
	double editModeTrigger = m_actionBackend->getDeltaByAction(
		InputActionsBackendSystem::Actions_TRIGGER_EDIT_MODE);
	if(editModeTrigger != 0.0)
	{
		m_world->getOutputLogger()->write(("Edit mode trigger toggle " 
			+ toString(editModeTrigger) + "\n").c_str());
		input.toggleEditMode = true;
	}
	
	return input;
}

void ShipInputProcessingSystem::updateHudKeymappings(bool p_useGamepad)
{
	if (m_hudSystem->isInited())
	{
		string left="L";
		string right="R";
		string front="F";
		string bottom="B";
		Control* ctrl = NULL;
		int pos = INT_MAX;
		if (p_useGamepad)
		{
			int size=sizeof(gamepadmap)/sizeof(gamepadmap[0]);
			// BOTTOM
			ctrl = m_actionBackend->findControlOfDeviceByAction(InputActionsBackendSystem::Actions_ACTIVATE_SLOT_1,
				InputHelper::InputDeviceTypes_XINPUT_DIGITAL);
			pos = ctrl->getControlEnum();
			if (ctrl && pos<size) bottom = gamepadmap[pos];
			// UP
			ctrl = m_actionBackend->findControlOfDeviceByAction(InputActionsBackendSystem::Actions_ACTIVATE_SLOT_2,
				InputHelper::InputDeviceTypes_XINPUT_DIGITAL);
			pos = ctrl->getControlEnum();
			if (ctrl && pos<size) front = gamepadmap[pos];
			// LEFT
			ctrl = m_actionBackend->findControlOfDeviceByAction(InputActionsBackendSystem::Actions_ACTIVATE_SLOT_3,
				InputHelper::InputDeviceTypes_XINPUT_DIGITAL);
			pos = ctrl->getControlEnum();
			if (ctrl && pos<size) left = gamepadmap[pos];
			// RIGHT
			ctrl = m_actionBackend->findControlOfDeviceByAction(InputActionsBackendSystem::Actions_ACTIVATE_SLOT_4,
				InputHelper::InputDeviceTypes_XINPUT_DIGITAL);
			pos = ctrl->getControlEnum();
			if (ctrl && pos<size) right = gamepadmap[pos];
		}
		else
		{
			int size=sizeof(keymap)/sizeof(keymap[0]);
			// BOTTOM
			ctrl = m_actionBackend->findControlOfDeviceByAction(InputActionsBackendSystem::Actions_ACTIVATE_SLOT_1,
				InputHelper::InputDeviceTypes_KEYBOARD);
			pos = ctrl->getControlEnum();
			if (ctrl && pos<size) bottom = keymap[pos];
			// UP
			ctrl = m_actionBackend->findControlOfDeviceByAction(InputActionsBackendSystem::Actions_ACTIVATE_SLOT_2,
				InputHelper::InputDeviceTypes_KEYBOARD);
			pos = ctrl->getControlEnum();
			if (ctrl && pos<size) front = keymap[pos];
			// LEFT
			ctrl = m_actionBackend->findControlOfDeviceByAction(InputActionsBackendSystem::Actions_ACTIVATE_SLOT_3,
				InputHelper::InputDeviceTypes_KEYBOARD);
			pos = ctrl->getControlEnum();
			if (ctrl && pos<size) left = keymap[pos];
			// RIGHT
			ctrl = m_actionBackend->findControlOfDeviceByAction(InputActionsBackendSystem::Actions_ACTIVATE_SLOT_4,
				InputHelper::InputDeviceTypes_KEYBOARD);
			pos = ctrl->getControlEnum();
			if (ctrl && pos<size) right = keymap[pos];
		}
		//
		m_hudSystem->setHUDData(HudSystem::MAPPING_LEFT,left.c_str());
		m_hudSystem->setHUDData(HudSystem::MAPPING_RIGHT,right.c_str());
		m_hudSystem->setHUDData(HudSystem::MAPPING_UP,front.c_str());
		m_hudSystem->setHUDData(HudSystem::MAPPING_DOWN,bottom.c_str());
	}

}