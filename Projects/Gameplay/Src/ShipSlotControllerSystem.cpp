#include "ShipSlotControllerSystem.h"
#include <Control.h>
#include "InputActionsBackendSystem.h"
#include "InputBackendSystem.h"
#include "HighlightSlotPacket.h"
#include "SimpleEventPacket.h"
#include <TcpClient.h>
#include "AudioBackendSystem.h"
#include <Globals.h>

SlotInputControllerSystem::SlotInputControllerSystem(InputBackendSystem* p_inputBackend,
										 TcpClient* p_client)
	: EntitySystem( SystemType::SlotInputController )
{
	m_inputBackend = p_inputBackend;
	m_client = p_client;
}

SlotInputControllerSystem::~SlotInputControllerSystem()
{
}

void SlotInputControllerSystem::handleSlotSelection(bool p_editMode)
{
	if (p_editMode)
	{
		if (m_keyboardRotateModuleSlots[0]->getDelta() > 0)
		{
			sendSlotRotationAdd();
		}
		else if (m_keyboardRotateModuleSlots[1]->getDelta() > 0)
		{
			sendSlotRotationSub();
		}
		else if (m_keyboardRotateModuleSlots[0]->getDelta() < 0
			|| m_keyboardRotateModuleSlots[1]->getDelta() < 0)
		{
			sendSlotRotationNone();
		}
		if (m_keyboardRotateModuleSlots[2]->getDelta() > 0)
		{
			sendSlot90Add();
		}
		else if (m_keyboardRotateModuleSlots[3]->getDelta() > 0)
		{
			sendSlot90Sub();
		}
	}
	else
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			if (m_keyboardModuleSlots[i]->getDelta() > 0 ||
				m_gamepadModuleSlots[i]->getDelta() > 0)
			{
				//Highlight slot
				sendModuleSlotHighlight(i);
				AudioBackendSystem* audioBackend = static_cast<AudioBackendSystem*>(
					m_world->getSystem(SystemType::AudioBackendSystem));
				audioBackend->playSoundEffect(TESTSOUNDEFFECTPATH,
					"WARFARE M-16 RELOAD RELOAD FULL CLIP MAGAZINE 01.wav");
			}
		}
		if (m_mouseModuleActivation->getDelta() > 0 ||
			m_gamepadModuleActivation->getDelta()>0)
		{
			sendSlotActivation();
		}
		else if (m_mouseModuleActivation->getDelta() < 0 ||
			m_gamepadModuleActivation->getDelta() < 0)
		{
			sendSlotDeactivation();
		}
	}
}


void SlotInputControllerSystem::process()
{
	
}

void SlotInputControllerSystem::initialize()
{
	m_actionBackend = static_cast<InputActionsBackendSystem*>(m_world->getSystem(
		SystemType::InputActionsBackendSystem));
}

void SlotInputControllerSystem::sendModuleSlotHighlight(int p_slot)
{
	HighlightSlotPacket packet;
	packet.id = p_slot;

	m_client->sendPacket( packet.pack() );
}


void SlotInputControllerSystem::sendModuleSlotHighlightDeactivate( int p_slot )
{
	HighlightSlotPacket packet(HighlightSlotPacket::UNHIGHLIGHT_SLOT,p_slot);
	m_client->sendPacket( packet.pack() );
}

void SlotInputControllerSystem::sendModuleSlotHighlightDeactivateAll()
{
	HighlightSlotPacket packet(HighlightSlotPacket::UNHIGHLIGHT_ALL);
	m_client->sendPacket( packet.pack() );
}


void SlotInputControllerSystem::sendSlotActivation()
{
	SimpleEventPacket packet;
	packet.type = SimpleEventType::ACTIVATE_MODULE;

	m_client->sendPacket( packet.pack() );
}

void SlotInputControllerSystem::sendSlotDeactivation()
{
	SimpleEventPacket packet;
	packet.type = SimpleEventType::DEACTIVATE_MODULE;

	m_client->sendPacket( packet.pack() );
}
void SlotInputControllerSystem::sendSlotRotationAdd()
{
	SimpleEventPacket packet;
	packet.type = SimpleEventType::ROTATE_ADD;

	m_client->sendPacket( packet.pack() );
}
void SlotInputControllerSystem::sendSlotRotationSub()
{
	SimpleEventPacket packet;
	packet.type = SimpleEventType::ROTATE_SUB;

	m_client->sendPacket( packet.pack() );
}
void SlotInputControllerSystem::sendSlotRotationNone()
{
	SimpleEventPacket packet;
	packet.type = SimpleEventType::ROTATE_NONE;

	m_client->sendPacket( packet.pack() );
}
void SlotInputControllerSystem::sendSlot90Sub()
{
	SimpleEventPacket packet;
	packet.type = SimpleEventType::ROTATE_90_SUB;

	m_client->sendPacket( packet.pack() );
}
void SlotInputControllerSystem::sendSlot90Add()
{
	SimpleEventPacket packet;
	packet.type = SimpleEventType::ROTATE_90_ADD;

	m_client->sendPacket( packet.pack() );
}

void SlotInputControllerSystem::initKeyboard()
{
	m_keyboardModuleSlots[0] = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_1);
	m_keyboardModuleSlots[1] = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_2);
	m_keyboardModuleSlots[2] = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_3);
	m_keyboardModuleSlots[3] = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_4);

	m_keyboardRotateModuleSlots[0] = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_NUMPAD_7);
	m_keyboardRotateModuleSlots[1] = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_NUMPAD_8);
	m_keyboardRotateModuleSlots[2] = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_NUMPAD_4);
	m_keyboardRotateModuleSlots[3] = m_inputBackend->getControlByEnum(
		InputHelper::KeyboardKeys_NUMPAD_5);
}


void SlotInputControllerSystem::initGamepad()
{
	m_gamepadModuleSlots[0] = m_inputBackend->getControlByEnum(
		InputHelper::Xbox360Digitals_BTN_A);
	m_gamepadModuleSlots[1] = m_inputBackend->getControlByEnum(
		InputHelper::Xbox360Digitals_BTN_Y);
	m_gamepadModuleSlots[2] = m_inputBackend->getControlByEnum(
		InputHelper::Xbox360Digitals_BTN_X);
	m_gamepadModuleSlots[3] = m_inputBackend->getControlByEnum(
		InputHelper::Xbox360Digitals_BTN_B);

	m_gamepadModuleActivation = m_inputBackend->getControlByEnum(
		InputHelper::Xbox360Analogs_TRIGGER_L);
}


void SlotInputControllerSystem::initMouse()
{
	m_mouseModuleActivation = m_inputBackend->getControlByEnum(
		InputHelper::MouseButtons_0);
}
