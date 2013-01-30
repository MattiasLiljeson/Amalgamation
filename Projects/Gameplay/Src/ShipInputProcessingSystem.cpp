#include "ShipInputProcessingSystem.h"
#include "InputBackendSystem.h"
#include "Control.h"
#include <ValueClamp.h>
#include "AntTweakBarWrapper.h"
#include "HighlightSlotPacket.h"
#include "SimpleEventPacket.h"
#include "AudioBackendSystem.h"
#include <Globals.h>


ShipInputProcessingSystem::ShipInputProcessingSystem(InputBackendSystem* p_inputBackend, TcpClient* p_client) :
										EntitySystem( SystemType::ShipInputProcessingSystem )
{
	m_angleInputMultiplier = 1000;

	m_controllerEpsilon = 0.15f;
	m_editSwitchTrigReleased = true;
	m_inputBackend = p_inputBackend;
	m_client = p_client;
}

void ShipInputProcessingSystem::initialize()
{
	initGamePad();
	initMouse();
	initKeyboard();

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
		InputHelper::Xbox360Analogs_THUMB_LX_POSITIVE);
	m_gamepadHorizontalNegative	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_LX_NEGATIVE);
	m_gamepadVerticalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_LY_POSITIVE);
	m_gamepadVerticalNegative	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_LY_NEGATIVE);

	m_gamepadRollRight	= m_inputBackend->getControlByEnum( InputHelper::Xbox360Digitals_SHOULDER_PRESS_R);
	m_gamepadRollLeft	= m_inputBackend->getControlByEnum( InputHelper::Xbox360Digitals_SHOULDER_PRESS_L);

	m_gamepadThrust	= m_inputBackend->getControlByEnum( InputHelper::Xbox360Analogs_TRIGGER_R);

	m_gamepadStrafeHorizontalPositive	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_RX_POSITIVE);
	m_gamepadStrafeHorizontalNegative	= m_inputBackend->getControlByEnum( 
		InputHelper::Xbox360Analogs_THUMB_RX_NEGATIVE);
	m_gamepadStrafeVerticalPositive	= m_inputBackend->getControlByEnum(
		InputHelper::Xbox360Analogs_THUMB_RY_POSITIVE);
	m_gamepadStrafeVerticalNegative	= m_inputBackend->getControlByEnum(
		InputHelper::Xbox360Analogs_THUMB_RY_NEGATIVE);

	m_gamepadEditModeTrig = m_inputBackend->getControlByEnum( InputHelper::Xbox360Digitals_BTN_BACK );
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

	//Added by Anton 15/1 - 13
	m_mouseModuleActivation = m_inputBackend->getControlByEnum(
		InputHelper::MouseButtons_0);
}

void ShipInputProcessingSystem::initKeyboard()
{
	m_keyboardRollRight = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_D);
	m_keyboardRollLeft =m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_A);

	m_keyboardThrust = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_W);

	m_keyboardStrafeVerticalPos = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_O);
	m_keyboardStrafeVerticalNeg = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_L);
	m_keyboarStrafeHorizontalPos = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_E);
	m_keyboarStrafeHorizontalNeg = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_Q);

	m_keyboardEditModeTrig = m_inputBackend->getControlByEnum( InputHelper::KeyboardKeys_C );

	//Added by Anton 15/1 - 13
	m_keyboardModuleSlots[0] = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_1);
	m_keyboardModuleSlots[1] = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_2);
	m_keyboardModuleSlots[2] = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_3);
	m_keyboardModuleSlots[3] = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_4);
}

float* ShipInputProcessingSystem::getControllerEpsilonPointer()
{
	return &m_controllerEpsilon;
}

ShipInputProcessingSystem::RawInputForces ShipInputProcessingSystem::readAllInput()
{
	RawInputForces input;
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

	input.shPositive = m_gamepadStrafeHorizontalPositive->getStatus();
	input.shPositive += m_keyboarStrafeHorizontalPos->getStatus();
	saturate(input.shPositive);

	input.shNegative = m_gamepadStrafeHorizontalNegative->getStatus();
	input.shNegative += m_keyboarStrafeHorizontalNeg->getStatus();
	saturate(input.shNegative);

	input.svPositive = m_gamepadStrafeVerticalPositive->getStatus();
	input.svPositive += m_keyboardStrafeVerticalPos->getStatus();
	saturate(input.svPositive);

	input.svNegative = m_gamepadStrafeVerticalNegative->getStatus();
	input.svNegative += m_keyboardStrafeVerticalNeg->getStatus();
	saturate(input.svNegative);

	input.rRight = m_gamepadRollRight->getStatus();
	input.rRight += m_keyboardRollRight->getStatus();
	saturate(input.rRight);

	input.rLeft = m_gamepadRollLeft->getStatus();
	input.rLeft += m_keyboardRollLeft->getStatus();
	saturate(input.rLeft);

	input.thrust =  m_gamepadThrust->getStatus();
	input.thrust += m_keyboardThrust->getStatus();
	saturate(input.thrust);

	double etv = m_gamepadEditModeTrig->getStatus();
	etv += m_keyboardEditModeTrig->getStatus();
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

	//Added by Anton 15/1 - 13
	// Please remove this!
	// Not input... Make separate system for slot highlight
	for (unsigned int i = 0; i < 4; i++)
	{
		if (m_keyboardModuleSlots[i]->getDelta() > 0)
		{
			//Highlight slot
			sendModuleSlotHighlight(i);
			AudioBackendSystem* audioBackend = static_cast<AudioBackendSystem*>(
				m_world->getSystem(SystemType::AudioBackendSystem));
			audioBackend->playSoundEffect(TESTSOUNDEFFECTPATH,
				"WARFARE M-16 RELOAD RELOAD FULL CLIP MAGAZINE 01.wav");

		}
	}
	// =============================
	
	if (m_mouseModuleActivation->getDelta() > 0)
	{
		//Send activation
		sendSlotActivation();
	}
	else if (m_mouseModuleActivation->getDelta() < 0)
	{
		//Send deactivation
		sendSlotDeactivation();
	}

	return input;
}



void ShipInputProcessingSystem::updateAntTweakBar(const ResultingInputForces& p_input)
{
	// Calibrate the Gamepad's analogue sticks when pressing the C key.
	if( m_inputBackend->getControlByEnum( InputHelper::KeyboardKeys_C )->getDelta() >= 0.5 && false )
	{
		
	}

	//// Update the analogue sticks for anttweakbar.
	//m_leftStickDir[0] = p_input.horizontalInput;
	//m_leftStickDir[1] = p_input.verticalInput;
	//m_leftStickDir[2] = 0.5f;

	//m_rightStickDir[0] = p_input.strafeHorizontalInput;
	//m_rightStickDir[1] = p_input.strafeVerticalInput;
	//m_rightStickDir[2] = 0.5f;

	// Update the corrected values for the sticks, in for anttweakbar.
	//m_leftStickDirWithCorrection[0] = p_input.horizontalInput + m_leftStickCorrection[0];
	//m_leftStickDirWithCorrection[1] = p_input.verticalInput + m_leftStickCorrection[1];
	//m_leftStickDirWithCorrection[2] = 0.5f;

	//m_rightStickDirWithCorrection[0] = p_input.strafeHorizontalInput + m_rightStickCorrection[0];
	//m_rightStickDirWithCorrection[1] = p_input.strafeVerticalInput + m_rightStickCorrection[1];
	//m_rightStickDirWithCorrection[2] = 0.5f;
}

void ShipInputProcessingSystem::sendModuleSlotHighlight(int p_slot)
{
	HighlightSlotPacket packet;
	packet.id = p_slot;

	m_client->sendPacket( packet.pack() );
}
void ShipInputProcessingSystem::sendSlotActivation()
{
	SimpleEventPacket packet;
	packet.type = SimpleEventType::ACTIVATE_MODULE;

	m_client->sendPacket( packet.pack() );
}
void ShipInputProcessingSystem::sendSlotDeactivation()
{
	SimpleEventPacket packet;
	packet.type = SimpleEventType::DEACTIVATE_MODULE;

	m_client->sendPacket( packet.pack() );
}
