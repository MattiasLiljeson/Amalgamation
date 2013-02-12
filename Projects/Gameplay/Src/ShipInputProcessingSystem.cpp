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


ShipInputProcessingSystem::ShipInputProcessingSystem(InputBackendSystem* p_inputBackend) :
										EntitySystem( SystemType::ShipInputProcessingSystem )
{
	m_angleInputMultiplier = 1000.0f;

	m_controllerEpsilon = 0.15f;
	m_editSwitchTrigReleased = true;
	m_inputBackend = p_inputBackend;
}

void ShipInputProcessingSystem::initialize()
{
	m_actionBackend = static_cast<InputActionsBackendSystem*>(m_world->getSystem(
		SystemType::InputActionsBackendSystem));
	initGamePad();
	initMouse();

	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::INPUT,
		"ControllerEpsilon", TwType::TW_TYPE_FLOAT, getControllerEpsilonPointer(), 
		"min=0.0 max=1.0 step=0.05" );

	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::INPUT,
		"MouseSensitivity", TwType::TW_TYPE_FLOAT, &m_angleInputMultiplier, "");
}

void ShipInputProcessingSystem::process()
{
	// Fetch the status of the various input methods.
	RawInputForces rawInput = readAllInput();
	// processed input
	m_processedInput = rawInput;

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

void ShipInputProcessingSystem::initGamePad()
{
	m_gamepadHorizontalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_LX_POSITIVE);
	m_gamepadHorizontalNegative	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_LX_NEGATIVE);
	m_gamepadVerticalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_LY_POSITIVE);
	m_gamepadVerticalNegative	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_LY_NEGATIVE);
}


void ShipInputProcessingSystem::initMouse()
{
	m_mouseHorizontalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::MouseAxes_X_POSITIVE);
	m_mouseHorizontalNegative	= m_inputBackend->getControlByEnum(
		InputHelper::MouseAxes_X_NEGATIVE);
	m_mouseVerticalPositive		= m_inputBackend->getControlByEnum( 
		InputHelper::MouseAxes_Y_POSITIVE);
	m_mouseVerticalNegative		= m_inputBackend->getControlByEnum( 
		InputHelper::MouseAxes_Y_NEGATIVE);
}

float* ShipInputProcessingSystem::getControllerEpsilonPointer()
{
	return &m_controllerEpsilon;
}

ShipInputProcessingSystem::RawInputForces ShipInputProcessingSystem::readAllInput()
{
	RawInputForces input;

	// rotate

	input.hPositive = m_gamepadHorizontalPositive->getStatus();
	input.hPositive += m_mouseHorizontalPositive->getStatus()*m_angleInputMultiplier;
	saturate(input.hPositive); // ?

	input.hNegative = m_gamepadHorizontalNegative->getStatus();
	input.hNegative += m_mouseHorizontalNegative->getStatus()*m_angleInputMultiplier;
	saturate(input.hNegative); // ?

	input.vPositive = m_gamepadVerticalPositive->getStatus();
	input.vPositive += m_mouseVerticalPositive->getStatus()*m_angleInputMultiplier;
	saturate(input.vPositive); // ?

	input.vNegative = m_gamepadVerticalNegative->getStatus();
	input.vNegative += m_mouseVerticalNegative->getStatus()*m_angleInputMultiplier;
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

	// edit mode

	double etv = m_actionBackend->getStatusByAction(
		InputActionsBackendSystem::Actions_TRIGGER_EDIT_MODE);
	input.stateSwitchTrig=saturate(etv)>0.5;

	if (m_editSwitchTrigReleased)
	{
		if (input.stateSwitchTrig)
		{
			m_editSwitchTrigReleased = false;
		}
	}
	else
	{
		if (!input.stateSwitchTrig)
		{
			m_editSwitchTrigReleased = true;
		}
		input.stateSwitchTrig=false;
	}
	
	return input;
}
