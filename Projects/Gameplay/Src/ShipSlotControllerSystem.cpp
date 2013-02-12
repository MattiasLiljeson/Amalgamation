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

void SlotInputControllerSystem::handleSlotSelection()
{
	for (unsigned int i = 0; i < 4; i++)
	{
		if (m_actionBackend->getDeltaByAction(
			static_cast<InputActionsBackendSystem::Actions>(
			(int)InputActionsBackendSystem::Actions_ACTIVATE_SLOT_1 +
			i) ) > 0.0)
		{
			//Highlight slot
			sendModuleSlotHighlight(i);
			AudioBackendSystem* audioBackend = static_cast<AudioBackendSystem*>(
				m_world->getSystem(SystemType::AudioBackendSystem));
			audioBackend->playSoundEffect(TESTSOUNDEFFECTPATH,
				"WARFARE M-16 RELOAD RELOAD FULL CLIP MAGAZINE 01.wav");
		}
	}
	
	if (m_actionBackend->getDeltaByAction(
		InputActionsBackendSystem::Actions_ACTIVATE_MODULE) > 0)
	{
		sendSlotActivation();
	}
	else if (m_actionBackend->getDeltaByAction(
		InputActionsBackendSystem::Actions_ACTIVATE_MODULE) < 0)
	{
		sendSlotDeactivation();
	}

	if (m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_ROTATE_MODULE_RIGHT) > 0)
	{
		sendSlotRotationAdd();
	}
	else if (m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_ROTATE_MODULE_LEFT) > 0)
	{
		sendSlotRotationSub();
	}
	else if (m_actionBackend->getDeltaByAction(
		InputActionsBackendSystem::Actions_ROTATE_MODULE_RIGHT) < 0 ||
		m_actionBackend->getDeltaByAction(
		InputActionsBackendSystem::Actions_ROTATE_MODULE_LEFT) < 0)
	{
		sendSlotRotationNone();
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