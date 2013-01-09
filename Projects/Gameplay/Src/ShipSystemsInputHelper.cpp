#include "ShipSystemsInputHelper.h"
#include "InputBackendSystem.h"
#include "Control.h"

ShipSystemsInputHelper::ShipSystemsInputHelper()
{
	m_mouseSensitivity = 2000;

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
}

void ShipSystemsInputHelper::initGamePad(InputBackendSystem* p_inputBackend)
{
	m_gamepadHorizontalPositive	= p_inputBackend->getControlByEnum( 
		InputHelper::THUMB_LX_POSITIVE);
	m_gamepadHorizontalNegative	= p_inputBackend->getControlByEnum( 
		InputHelper::THUMB_LX_NEGATIVE);
	m_gamepadVerticalPositive	= p_inputBackend->getControlByEnum( 
		InputHelper::THUMB_LY_POSITIVE);
	m_gamepadVerticalNegative	= p_inputBackend->getControlByEnum( 
		InputHelper::THUMB_LY_NEGATIVE);

	m_gamepadRollRight	= p_inputBackend->getControlByEnum( InputHelper::SHOULDER_PRESS_R);
	m_gamepadRollLeft	= p_inputBackend->getControlByEnum( InputHelper::SHOULDER_PRESS_L);

	m_gamepadThrust	= p_inputBackend->getControlByEnum( InputHelper::TRIGGER_R);

	m_gamepadStrafeHorizontalPositive	= p_inputBackend->getControlByEnum( 
		InputHelper::THUMB_RX_POSITIVE);
	m_gamepadStrafeHorizontalNegative	= p_inputBackend->getControlByEnum( 
		InputHelper::THUMB_RX_NEGATIVE);
	m_gamepadStrafeVerticalPositive	= p_inputBackend->getControlByEnum(
		InputHelper::THUMB_RY_POSITIVE);
	m_gamepadStrafeVerticalNegative	= p_inputBackend->getControlByEnum(
		InputHelper::THUMB_RY_NEGATIVE);

	m_gamepadEditModeTrig = p_inputBackend->getControlByEnum( InputHelper::BTN_BACK );
}


void ShipSystemsInputHelper::initMouse(InputBackendSystem* p_inputBackend)
{
	m_mouseHorizontalPositive	= p_inputBackend->getControlByEnum( 
		InputHelper::MOUSE_AXIS::X_POSITIVE);
	m_mouseHorizontalNegative	= p_inputBackend->getControlByEnum(
		InputHelper::MOUSE_AXIS::X_NEGATIVE);
	m_mouseVerticalPositive		= p_inputBackend->getControlByEnum( 
		InputHelper::MOUSE_AXIS::Y_POSITIVE);
	m_mouseVerticalNegative		= p_inputBackend->getControlByEnum( 
		InputHelper::MOUSE_AXIS::Y_NEGATIVE);
}

void ShipSystemsInputHelper::initKeyboard(InputBackendSystem* p_inputBackend)
{
	m_keyboardRollRight = p_inputBackend->getControlByEnum(
		InputHelper::KEY_D);
	m_keyboardRollLeft =p_inputBackend->getControlByEnum(
		InputHelper::KEY_A);

	m_keyboardThrust = p_inputBackend->getControlByEnum(
		InputHelper::KEY_W);

	m_keyboardStrafeVerticalPos = p_inputBackend->getControlByEnum(
		InputHelper::KEY_O);
	m_keyboardStrafeVerticalNeg = p_inputBackend->getControlByEnum(
		InputHelper::KEY_L);
	m_keyboarStrafeHorizontalPos = p_inputBackend->getControlByEnum(
		InputHelper::KEY_E);
	m_keyboarStrafeHorizontalNeg = p_inputBackend->getControlByEnum(
		InputHelper::KEY_Q);

	m_keyboardEditModeTrig = p_inputBackend->getControlByEnum( InputHelper::KEY_C );
}

float* ShipSystemsInputHelper::getControllerEpsilonPointer()
{
	return &m_controllerEpsilon;
}

void ShipSystemsInputHelper::readAllTheInput(RawInputForces& p_outInput)
{
	p_outInput.hPositive = m_gamepadHorizontalPositive->getStatus();
	p_outInput.hPositive += m_mouseHorizontalPositive->getStatus()*m_mouseSensitivity;

	p_outInput.hNegative = m_gamepadHorizontalNegative->getStatus();
	p_outInput.hNegative += m_mouseHorizontalNegative->getStatus()*m_mouseSensitivity;

	p_outInput.vPositive = m_gamepadVerticalPositive->getStatus();
	p_outInput.vPositive += m_mouseVerticalPositive->getStatus()*m_mouseSensitivity;

	p_outInput.vNegative = m_gamepadVerticalNegative->getStatus();
	p_outInput.vNegative += m_mouseVerticalNegative->getStatus()*m_mouseSensitivity;

	p_outInput.shPositive = m_gamepadStrafeHorizontalPositive->getStatus();
	p_outInput.shPositive += m_keyboarStrafeHorizontalPos->getStatus();

	p_outInput.shNegative = m_gamepadStrafeHorizontalNegative->getStatus();
	p_outInput.shNegative += m_keyboarStrafeHorizontalNeg->getStatus();

	p_outInput.svPositive = m_gamepadStrafeVerticalPositive->getStatus();
	p_outInput.svPositive += m_keyboardStrafeVerticalPos->getStatus();

	p_outInput.svNegative = m_gamepadStrafeVerticalNegative->getStatus();
	p_outInput.svNegative += m_keyboardStrafeVerticalNeg->getStatus();

	p_outInput.rRight = m_gamepadRollRight->getStatus();
	p_outInput.rRight += m_keyboardRollRight->getStatus();

	p_outInput.rLeft = m_gamepadRollLeft->getStatus();
	p_outInput.rLeft += m_keyboardRollLeft->getStatus();

	p_outInput.thrust =  m_gamepadThrust->getStatus();
	p_outInput.thrust += m_keyboardThrust->getStatus();

	p_outInput.editSwitchTrig = m_keyboardEditModeTrig->getStatus();
}



void ShipSystemsInputHelper::updateAntTweakBar(const ResultingInputForces& p_input,
											   InputBackendSystem* p_inputBackend)
{
	// Calibrate the Gamepad's analogue sticks when pressing the C key.
	if( p_inputBackend->getControlByEnum( InputHelper::KEY_C )->getDelta() >= 0.5 )
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
