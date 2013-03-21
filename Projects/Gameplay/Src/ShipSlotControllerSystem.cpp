#include "ShipSlotControllerSystem.h"
#include <Control.h>
#include "InputActionsBackendSystem.h"
#include "InputBackendSystem.h"
#include "HighlightSlotPacket.h"
#include "SimpleEventPacket.h"
#include <TcpClient.h>
#include "AudioBackendSystem.h"
#include <Globals.h>
#include "ParticleSystemsComponent.h"
#include "ParticleSystemAndTexture.h"
#include <AglParticleSystem.h>
#include "SlotMarkerSystem.h"
#include "SoundComponent.h"

SlotInputControllerSystem::SlotInputControllerSystem(InputBackendSystem* p_inputBackend,
										 TcpClient* p_client)
	: EntitySystem( SystemType::SlotInputController )
{
	m_inputBackend = p_inputBackend;
	m_client = p_client;
	m_previousModeWasEditMode = false;
	m_previousHighlight=-1;
}

SlotInputControllerSystem::~SlotInputControllerSystem()
{
}

void SlotInputControllerSystem::handleSlotSelection(bool p_editMode)
{
	if (p_editMode)
	{
		if (m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_ROTATE_MODULE_RIGHT) > 0)
		{
			sendSlotRotationAdd();
		}
		else if (m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_ROTATE_MODULE_LEFT) > 0)
		{
			sendSlotRotationSub();
		}
		else if (m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_ROTATE_MODULE_RIGHT) < 0
			|| m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_ROTATE_MODULE_LEFT) < 0)
		{
			sendSlotRotationNone();
		}
		if (m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_ROTATE90_MODULE_RIGHT) > 0)
		{
			sendSlot90Add();
		}
		else if (m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_ROTATE90_MODULE_LEFT) > 0)
		{
			sendSlot90Sub();
		}
		else if (m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_TOGGLE_DESIRED_SLOT) > 0)
		{
			sendPreferredSlotToggle();
		}
		m_previousModeWasEditMode = true;
	}
	else
	{
		Entity* myShip = m_world->getEntityManager()->getFirstEntityByComponentType(
			ComponentType::TAG_MyShip);
		int highlight = -1;

		// history based
		if (m_previousModeWasEditMode)
		{
			highlight=m_previousHighlight;
			m_previousModeWasEditMode=false;
		}
		// input based
		if (m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_ACTIVATE_SLOT_1) > 0)
		{
			highlight=0;
		}
		if (m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_ACTIVATE_SLOT_2) > 0)
		{
			highlight=1;
		}
		if (m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_ACTIVATE_SLOT_3) > 0)
		{
			highlight=2;
		}
		if (m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_ACTIVATE_SLOT_4) > 0)
		{
			highlight=3;
		}

		if (highlight >= 0)
		{
			m_previousHighlight=highlight;
			//Highlight slot

			static_cast<SoundComponent*>(m_slotActivationSound->
				getComponent(ComponentType::SoundComponent))->
				setSoundHeadersPlayingState(AudioHeader::PLAY,AudioHeader::AMBIENT,"SlotActivation");

			sendModuleSlotHighlight(highlight);
		}
	
		if (m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_ACTIVATE_MODULE) > 0)
		{
			sendSlotActivation();
		}
		else if (m_actionBackend->getDeltaByAction(InputActionsBackendSystem::Actions_ACTIVATE_MODULE) < 0)
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

	m_slotActivationSound = m_world->createEntity();

	SoundComponent* soundComp = new SoundComponent();
	AudioHeader* audioHeader = new AudioHeader(AudioHeader::AMBIENT,"SlotActivation");
	audioHeader->file = "Slot_selected.wav";
	audioHeader->path = SOUNDEFFECTPATH;
	audioHeader->playInterval = AudioHeader::ONCE;
	soundComp->addAudioHeader(audioHeader);

	m_slotActivationSound->addComponent(soundComp);
	m_world->addEntity(m_slotActivationSound);
}

void SlotInputControllerSystem::sendModuleSlotHighlight(int p_slot)
{
	HighlightSlotPacket packet;
	packet.id = p_slot;

	m_client->sendPacket( packet.pack() );

	SlotMarkerSystem* sms = static_cast<SlotMarkerSystem*>(m_world->getSystem(SystemType::SlotMarkerSystem));

	sms->setSlot(p_slot);
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
void SlotInputControllerSystem::sendPreferredSlotToggle()
{
	SimpleEventPacket packet;
	packet.type = SimpleEventType::TOGGLE_PREFERRED_SLOT;

	m_client->sendPacket( packet.pack() );
}
