#include "ShipInputProcessingSystem.h"
#include "InputBackendSystem.h"
#include "Control.h"
#include <ValueClamp.h>
#include "AntTweakBarWrapper.h"


ShipInputProcessingSystem::ShipInputProcessingSystem(InputBackendSystem* p_inputBackend) :
										EntitySystem( SystemType::ShipInputProcessingSystem )
{
	m_mouseSensitivity = 5000;

	m_controllerEpsilon = 0.15f;
	m_leftStickDir[0] = 0;
	m_leftStickDir[1] = 0;
	m_leftStickDir[2] = 0;
	m_rightStickDir[0] = 0;
	m_rightStickDir[1] = 0;
	m_rightStickDir[2] = 0;
	m_leftStickDirWithCorrection[0] = 0;
	m_leftStickDirWithCorrection[1] = 0;
	m_leftStickDirWithCorrection[2] = 0;
	m_rightStickDirWithCorrection[0] = 0;
	m_rightStickDirWithCorrection[1] = 0;
	m_rightStickDirWithCorrection[2] = 0;

	m_leftStickCorrection[0] = 0.1;
	m_leftStickCorrection[1] = 0.1;
	m_rightStickCorrection[0] = 0;
	m_rightStickCorrection[1] = 0;

	m_editSwitchTrigReleased = true;
	m_inputBackend = p_inputBackend;
}

void ShipInputProcessingSystem::initialize()
{
	initGamePad();
	initMouse();
	initKeyboard();

	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::INPUT,
		"ControllerEpsilon", TwType::TW_TYPE_FLOAT, getControllerEpsilonPointer(), 
		"min=0.0 max=1.0 step=0.05" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( AntTweakBarWrapper::INPUT,
		"Left Stick (raw)",	TwType::TW_TYPE_DIR3D, &m_leftStickDir, "" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( AntTweakBarWrapper::INPUT,
		"Right Stick (raw)", TwType::TW_TYPE_DIR3D, &m_rightStickDir, "" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( AntTweakBarWrapper::INPUT,
		"Left W/ Correction", TwType::TW_TYPE_DIR3D, &m_leftStickDirWithCorrection, "" );

	AntTweakBarWrapper::getInstance()->addReadOnlyVariable( AntTweakBarWrapper::INPUT,
		"Right W/ Correction", TwType::TW_TYPE_DIR3D, &m_rightStickDirWithCorrection, "" );

	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::INPUT,
		"MouseSensitivity", TwType::TW_TYPE_FLOAT, &m_mouseSensitivity, "");
}

void ShipInputProcessingSystem::process()
{
	// Fetch the status of the various input methods.
	RawInputForces rawInput;
	readAllTheInput(rawInput);
	// processed input
	m_processedInput = rawInput;

	// Apply correction vectors to the analogue sticks.
	m_processedInput.horizontalInput += static_cast<float>(m_leftStickCorrection[0]);
	m_processedInput.verticalInput += static_cast<float>(m_leftStickCorrection[1]);

	// Apply a threshold value to eliminate some of the analogue stick noise.
	if( abs(m_processedInput.horizontalInput) < m_controllerEpsilon ) 
		m_processedInput.horizontalInput = 0;

	if( abs(m_processedInput.verticalInput) < m_controllerEpsilon ) 
		m_processedInput.verticalInput = 0;

	if( abs(m_processedInput.strafeHorizontalInput) < m_controllerEpsilon ) 
		m_processedInput.strafeHorizontalInput = 0;

	if( abs(m_processedInput.strafeVerticalInput) < m_controllerEpsilon ) 
		m_processedInput.strafeVerticalInput = 0;


	updateAntTweakBar(m_processedInput);
}

ShipInputProcessingSystem::ResultingInputForces& ShipInputProcessingSystem::getProcessedInput()
{
	return m_processedInput;
}

void ShipInputProcessingSystem::initGamePad()
{
	m_gamepadHorizontalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::THUMB_LX_POSITIVE);
	m_gamepadHorizontalNegative	= m_inputBackend->getControlByEnum( 
		InputHelper::THUMB_LX_NEGATIVE);
	m_gamepadVerticalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::THUMB_LY_POSITIVE);
	m_gamepadVerticalNegative	= m_inputBackend->getControlByEnum( 
		InputHelper::THUMB_LY_NEGATIVE);

	m_gamepadRollRight	= m_inputBackend->getControlByEnum( InputHelper::SHOULDER_PRESS_R);
	m_gamepadRollLeft	= m_inputBackend->getControlByEnum( InputHelper::SHOULDER_PRESS_L);

	m_gamepadThrust	= m_inputBackend->getControlByEnum( InputHelper::TRIGGER_R);

	m_gamepadStrafeHorizontalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::THUMB_RX_POSITIVE);
	m_gamepadStrafeHorizontalNegative	= m_inputBackend->getControlByEnum( 
		InputHelper::THUMB_RX_NEGATIVE);
	m_gamepadStrafeVerticalPositive	= m_inputBackend->getControlByEnum(
		InputHelper::THUMB_RY_POSITIVE);
	m_gamepadStrafeVerticalNegative	= m_inputBackend->getControlByEnum(
		InputHelper::THUMB_RY_NEGATIVE);

	m_gamepadEditModeTrig = m_inputBackend->getControlByEnum( InputHelper::BTN_BACK );
}


void ShipInputProcessingSystem::initMouse()
{
	m_mouseHorizontalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::MOUSE_AXIS::X_POSITIVE);
	m_mouseHorizontalNegative	= m_inputBackend->getControlByEnum(
		InputHelper::MOUSE_AXIS::X_NEGATIVE);
	m_mouseVerticalPositive		= m_inputBackend->getControlByEnum( 
		InputHelper::MOUSE_AXIS::Y_POSITIVE);
	m_mouseVerticalNegative		= m_inputBackend->getControlByEnum( 
		InputHelper::MOUSE_AXIS::Y_NEGATIVE);
}

void ShipInputProcessingSystem::initKeyboard()
{
	m_keyboardRollRight = m_inputBackend->getControlByEnum(
		InputHelper::KEY_D);
	m_keyboardRollLeft =m_inputBackend->getControlByEnum(
		InputHelper::KEY_A);

	m_keyboardThrust = m_inputBackend->getControlByEnum(
		InputHelper::KEY_W);

	m_keyboardStrafeVerticalPos = m_inputBackend->getControlByEnum(
		InputHelper::KEY_O);
	m_keyboardStrafeVerticalNeg = m_inputBackend->getControlByEnum(
		InputHelper::KEY_L);
	m_keyboarStrafeHorizontalPos = m_inputBackend->getControlByEnum(
		InputHelper::KEY_E);
	m_keyboarStrafeHorizontalNeg = m_inputBackend->getControlByEnum(
		InputHelper::KEY_Q);

	m_keyboardEditModeTrig = m_inputBackend->getControlByEnum( InputHelper::KEY_C );
}

float* ShipInputProcessingSystem::getControllerEpsilonPointer()
{
	return &m_controllerEpsilon;
}

void ShipInputProcessingSystem::readAllTheInput(RawInputForces& p_outInput)
{
	p_outInput.hPositive = m_gamepadHorizontalPositive->getStatus();
	p_outInput.hPositive += m_mouseHorizontalPositive->getStatus()*m_mouseSensitivity;
	saturate(p_outInput.hPositive); // ?

	p_outInput.hNegative = m_gamepadHorizontalNegative->getStatus();
	p_outInput.hNegative += m_mouseHorizontalNegative->getStatus()*m_mouseSensitivity;
	saturate(p_outInput.hNegative); // ?

	p_outInput.vPositive = m_gamepadVerticalPositive->getStatus();
	p_outInput.vPositive += m_mouseVerticalPositive->getStatus()*m_mouseSensitivity;
	saturate(p_outInput.vPositive); // ?

	p_outInput.vNegative = m_gamepadVerticalNegative->getStatus();
	p_outInput.vNegative += m_mouseVerticalNegative->getStatus()*m_mouseSensitivity;
	saturate(p_outInput.vNegative); // ?

	p_outInput.shPositive = m_gamepadStrafeHorizontalPositive->getStatus();
	p_outInput.shPositive += m_keyboarStrafeHorizontalPos->getStatus();
	saturate(p_outInput.shPositive);

	p_outInput.shNegative = m_gamepadStrafeHorizontalNegative->getStatus();
	p_outInput.shNegative += m_keyboarStrafeHorizontalNeg->getStatus();
	saturate(p_outInput.shNegative);

	p_outInput.svPositive = m_gamepadStrafeVerticalPositive->getStatus();
	p_outInput.svPositive += m_keyboardStrafeVerticalPos->getStatus();
	saturate(p_outInput.svPositive);

	p_outInput.svNegative = m_gamepadStrafeVerticalNegative->getStatus();
	p_outInput.svNegative += m_keyboardStrafeVerticalNeg->getStatus();
	saturate(p_outInput.svNegative);

	p_outInput.rRight = m_gamepadRollRight->getStatus();
	p_outInput.rRight += m_keyboardRollRight->getStatus();
	saturate(p_outInput.rRight);

	p_outInput.rLeft = m_gamepadRollLeft->getStatus();
	p_outInput.rLeft += m_keyboardRollLeft->getStatus();
	saturate(p_outInput.rLeft);

	p_outInput.thrust =  m_gamepadThrust->getStatus();
	p_outInput.thrust += m_keyboardThrust->getStatus();
	saturate(p_outInput.thrust);

	double etv = m_gamepadEditModeTrig->getStatus();
	etv += m_keyboardEditModeTrig->getStatus();
	p_outInput.stateSwitchTrig=saturate(etv)>0.5;

	if (m_editSwitchTrigReleased)
	{
		if (p_outInput.stateSwitchTrig)
		{
			m_editSwitchTrigReleased = false;
		}
	}
	else
	{
		if (!p_outInput.stateSwitchTrig)
		{
			m_editSwitchTrigReleased = true;
		}
		p_outInput.stateSwitchTrig=false;
	}

	

}



void ShipInputProcessingSystem::updateAntTweakBar(const ResultingInputForces& p_input)
{
	// Calibrate the Gamepad's analogue sticks when pressing the C key.
	if( m_inputBackend->getControlByEnum( InputHelper::KEY_C )->getDelta() >= 0.5 )
	{
		m_leftStickCorrection[0] = -p_input.horizontalInput;
		m_leftStickCorrection[1] = -p_input.verticalInput;
		m_rightStickCorrection[0] = -p_input.strafeHorizontalInput;
		m_rightStickCorrection[1] = -p_input.strafeVerticalInput;
	}

	// Update the analogue sticks for anttweakbar.
	m_leftStickDir[0] = p_input.horizontalInput;
	m_leftStickDir[1] = p_input.verticalInput;
	m_leftStickDir[2] = 0.5f;

	m_rightStickDir[0] = p_input.strafeHorizontalInput;
	m_rightStickDir[1] = p_input.strafeVerticalInput;
	m_rightStickDir[2] = 0.5f;

	// Update the corrected values for the sticks, in for anttweakbar.
	m_leftStickDirWithCorrection[0] = p_input.horizontalInput + m_leftStickCorrection[0];
	m_leftStickDirWithCorrection[1] = p_input.verticalInput + m_leftStickCorrection[1];
	m_leftStickDirWithCorrection[2] = 0.5f;

	m_rightStickDirWithCorrection[0] = p_input.strafeHorizontalInput + m_rightStickCorrection[0];
	m_rightStickDirWithCorrection[1] = p_input.strafeVerticalInput + m_rightStickCorrection[1];
	m_rightStickDirWithCorrection[2] = 0.5f;
}
