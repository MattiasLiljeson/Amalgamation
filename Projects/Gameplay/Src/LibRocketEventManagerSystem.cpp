/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#pragma once

#include "LibRocketEventManagerSystem.h"
#include <Rocket/Core/Context.h>
#include <Rocket/Core/ElementDocument.h>
#include <Rocket/Core/ElementUtilities.h>
#include "EventHandler.h"
#include "EventInstancer.h"
#include <map>
#include <Globals.h>
#include <ToString.h>
#include <DebugUtil.h>
#include "libRocketBackendSystem.h"
#include "ClientConnectToServerSystem.h"
#include "GameState.h"
#include <TcpClient.h>
#include <Packet.h>
#include "PacketType.h"
#include "ClientStateSystem.h"
#include "ChangeStatePacket.h"
#include "SoundComponent.h"
#include "DisconnectPacket.h"
#include "ClientPacketHandlerSystem.h"

LibRocketEventManagerSystem::LibRocketEventManagerSystem(TcpClient* p_client)
	: EntitySystem(SystemType::LibRocketEventManagerSystem, 1, ComponentType::GameState)
{
	m_context = NULL;
	wantsToExit = false;
	m_eventHandler = NULL;
	m_currentDocId = "";
	m_client = p_client;
}

LibRocketEventManagerSystem::~LibRocketEventManagerSystem()
{
	shutdown();
}

void LibRocketEventManagerSystem::initialize()
{
	m_stateSystem = static_cast<ClientStateSystem*>(m_world->getSystem(
		SystemType::ClientStateSystem));
	auto rocketBackend = static_cast<LibRocketBackendSystem*>(
		m_world->getSystem(SystemType::LibRocketBackendSystem));

	m_context = rocketBackend->getContext();

	EventInstancer* eventInstancer = new EventInstancer(this);
	Rocket::Core::Factory::RegisterEventListenerInstancer(eventInstancer);
	eventInstancer->RemoveReference();

	m_menuEntity = m_world->createEntity();
	SoundComponent* soundComp = new SoundComponent();
	AudioHeader* header = new AudioHeader(AudioHeader::AMBIENT,"MenuOk");
	header->file = "Mine_Blip_v2.wav";
	header->volume = 0.3f;
	header->path = TESTSOUNDEFFECTPATH;
	soundComp->addAudioHeader(header);

	m_menuEntity->addComponent(soundComp);
	m_world->addEntity(m_menuEntity);
}

// Releases all event handlers registered with the manager.
void LibRocketEventManagerSystem::shutdown()
{
	m_eventHandlers.clear();
	m_eventHandler = NULL;
}

void LibRocketEventManagerSystem::registerEventHandler( EventHandler* p_handler )
{
	registerEventHandler(p_handler->getName().c_str(), p_handler);
}

// Registers a new event handler with the manager.
void LibRocketEventManagerSystem::registerEventHandler(const Rocket::Core::String& 
													   p_handlerName, 
													   EventHandler* p_handler)
{
	p_handler->connectToManager(this);
	// Release any handler bound under the same name.
	EventHandlerMap::iterator iterator = m_eventHandlers.find(p_handlerName);
	if (iterator != m_eventHandlers.end())
	{	//delete (*iterator).second;
		DEBUGWARNING(((
			toString("LibRocketEventManagerSystem::registerEventHandler\nAttempting to register EventHandler[")
			+ toString(p_handlerName.CString())
			+ toString("] which has already been registred in the EventManager.")).c_str()));
	}
	m_eventHandlers[p_handlerName] = p_handler;
}

EventHandler* LibRocketEventManagerSystem::unregisterEventHandler( 
	const Rocket::Core::String& p_handlerName )
{
	EventHandlerMap::iterator iterator = m_eventHandlers.find(p_handlerName);
	EventHandler* handler = NULL;
	if (iterator != m_eventHandlers.end())
	{
		handler = (*iterator).second;
		handler->connectToManager(NULL);
		m_eventHandlers[p_handlerName] = NULL;
	}
	return handler;
}

void LibRocketEventManagerSystem::clearDocumentStack()
{
	while (!m_docIdStack.empty())
	{
		auto document = m_context->GetDocument(m_docIdStack.top());
		m_docIdStack.pop();
		if (document->IsVisible())
		{
			document->Show(Rocket::Core::ElementDocument::NONE);
			document->Hide();
		}
	}
	
	m_currentDocId = "";
}

// Processes an event coming through from Rocket.
void LibRocketEventManagerSystem::processEvent(Rocket::Core::Event& p_event, 
											   const Rocket::Core::String& p_value)
{
	Rocket::Core::StringList commands;
	Rocket::Core::StringUtilities::ExpandString(commands, p_value, ';');
	for (size_t i = 0; i < commands.size(); ++i)
	{
		// Check for a generic 'load', 'exit' or 'modal' command.
		Rocket::Core::StringList values;
		Rocket::Core::StringUtilities::ExpandString(values, commands[i], ' ');

		if (values.empty())
			return;

		auto ownerDocument = p_event.GetTargetElement()->GetOwnerDocument();
		if (values[0] == "modal")
		{
		//	if (!ownerDocument->IsModal() && m_currentDocId == ownerDocument->GetId())
		//		ownerDocument->Show(Rocket::Core::ElementDocument::MODAL);
		}
		else if (values[0] == "goto" && values.size() > 1)
		{
			playConfirmSound();
			// Clear the stack from windows that are on top of the triggering one.
			clearStackUntilFoundDocId(ownerDocument->GetId());
			// If goto previous is specified, then hide this window, and open the
			// previous one saved on the stack
			if (values[1] == "previous")
			{
				// Pop current document id from the stack.
				m_docIdStack.pop();
				// Top and pop next document id. On loadWindow, it will be added again.
				// That's not pretty/efficient, but it isn't a problem either. // Alex
				Rocket::Core::String window = m_docIdStack.top();
				m_docIdStack.pop();
				if (loadWindow(window))
					ownerDocument->Hide();
			}
			// Load the window, and if successful hide the old window.
			else if (loadWindow(values[1]))
			{
				ownerDocument->Hide();
			}
		}
		else if (values[0] == "open" &&	values.size() > 1)
		{
			playConfirmSound();
			// Opens a window and pushes it to the stack, without hiding the parent window.
			// Clear other open windows on top.
			clearStackUntilFoundDocId(ownerDocument->GetId());
			loadWindow(values[1]);
			ownerDocument->Show(Rocket::Core::ElementDocument::NONE);
		}
		else if (values[0] == "clearStack"){
			clearDocumentStack();
		}
		else if (values[0] == "exit")
		{
			wantsToExit = true;
		}
		else if (values[0] == "connectToServer")
		{
			// "server_host" is the name attribute specified in the input element in the rml file.
			// "localhost" simply is provided as a default value, if the host isn't set. This could be left as "" as well.
			string server_address = p_event.GetParameter<Rocket::Core::String>
				("server_host", "localhost").CString();
			string server_port = p_event.GetParameter<Rocket::Core::String>
				("server_port", "1337").CString();
			string playerName = p_event.GetParameter<Rocket::Core::String>
				("player_name", "NotFound").CString();
				
			m_client->setPlayerName(playerName);
			auto sys = static_cast<ClientConnectToServerSystem*>(
				m_world->getSystem(SystemType::ClientConnectoToServerSystem));

			sys->setAddressAndConnect(server_address, server_port);
		}
		else if(values[0] == "start_game")
		{
			ChangeStatePacket letsRollPacket;
			letsRollPacket.m_gameState = GameStates::INITGAME;
			m_client->sendPacket(letsRollPacket.pack());
		}
		else if(values[0] == "host_server"){
			m_world->requestToHostServer();

			string server_port = p_event.GetParameter<Rocket::Core::String>
				("server_port", "1337").CString();

			string playerName = p_event.GetParameter<Rocket::Core::String>
				("player_name", "NotFound").CString();
			m_client->setPlayerName(playerName);

			auto sys = static_cast<ClientConnectToServerSystem*>(
				m_world->getSystem(SystemType::ClientConnectoToServerSystem));

			sys->setAddressAndConnect("127.0.0.1", server_port);
		}
		else if(values[0] == "leave_server"){
			DisconnectPacket dcPacket;
			dcPacket.playerID = m_client->getPlayerID();
			dcPacket.clientNetworkIdentity = m_client->getId();
			m_client->sendPacket(dcPacket.pack());
		}
		else if (values[0] == "reset_connection")
		{
			ownerDocument->Show(Rocket::Core::ElementDocument::NONE);
			ownerDocument->PushToBack();
			ownerDocument->Hide();
			auto sys = static_cast<ClientPacketHandlerSystem*>(
				m_world->getSystem(SystemType::ClientPacketHandlerSystem));
			sys->resetFromDisconnect();
		}

		else if(values[0] == "play_confirm"){
			playConfirmSound();
		}
	}
}

void LibRocketEventManagerSystem::processEntities( const vector<Entity*>& p_entities )
{
	if (wantsToExit){
		m_world->requestToShutDown();
	}
}

void LibRocketEventManagerSystem::playConfirmSound()
{
	auto soundComp = static_cast<SoundComponent*>
		(m_menuEntity->getComponent(ComponentType::SoundComponent));

	soundComp->getSoundHeaderByName(AudioHeader::AMBIENT,"MenuOk")->
		queuedPlayingState = AudioHeader::PLAY;
}

void LibRocketEventManagerSystem::playBackSound()
{

}

// Loads a window and binds the event handler for it.
bool LibRocketEventManagerSystem::loadWindow(const Rocket::Core::String& p_windowName)
{
	// Set the event handler for the new screen, if one has been registered.
	EventHandler* old_event_handler = m_eventHandler;
	EventHandlerMap::iterator iterator = m_eventHandlers.find(p_windowName);
	if (iterator != m_eventHandlers.end())
		m_eventHandler = (*iterator).second;
	else
		m_eventHandler = NULL;

	auto document = m_context->GetDocument(p_windowName);
	if (document == NULL)
	{
		DEBUGWARNING(((
			toString("LibRocketEventManagerSystem::loadWindow\nNo document with the body id\"")
			+ toString(p_windowName.CString())
			+ toString("\" has been loaded.")).c_str()));
		m_eventHandler = old_event_handler;
		return false;
	}

	// Add this document to the stack!
	m_currentDocId = p_windowName;
	m_docIdStack.push(p_windowName);

	document->Show();
	document->PullToFront();

	// Remove the caller's reference.
	//document->RemoveReference();
	return true;
}

void LibRocketEventManagerSystem::clearStackUntilFoundDocId( const Rocket::Core::String&  p_docId )
{
	if(m_docIdStack.size()){
		while (m_docIdStack.top() != p_docId)
		{	
			auto document = m_context->GetDocument(m_docIdStack.top());
			m_docIdStack.pop();
			if (document->IsVisible())
			{
				document->Show(Rocket::Core::ElementDocument::NONE);
				document->Hide();
			}
		}
	}
}