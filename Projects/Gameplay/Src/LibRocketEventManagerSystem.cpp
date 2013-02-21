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

LibRocketEventManagerSystem::LibRocketEventManagerSystem()
	: EntitySystem(SystemType::LibRocketEventManagerSystem, 1, ComponentType::GameState)
{
	m_context = NULL;
	wantsToExit = false;
	m_eventHandler = NULL;
	m_currentDocId = "";
	m_stateEntity	= NULL;
	m_stateComp		= NULL;
	m_localState	= NONE;
}

LibRocketEventManagerSystem::~LibRocketEventManagerSystem()
{
	shutdown();
}

void LibRocketEventManagerSystem::initialize()
{
	auto rocketBackend = static_cast<LibRocketBackendSystem*>(
		m_world->getSystem(SystemType::LibRocketBackendSystem));

	m_context = rocketBackend->getContext();

	EventInstancer* eventInstancer = new EventInstancer(this);
	Rocket::Core::Factory::RegisterEventListenerInstancer(eventInstancer);
	eventInstancer->RemoveReference();
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
		else if( m_localState == NOTCHANGED)
		{
			if (values[0] == "connectToServer")
			{
				// "server_host" is the name attribute specified in the input element in the rml file.
				// "localhost" simply is provided as a default value, if the host isn't set. This could be left as "" as well.
				string server_address = p_event.GetParameter<Rocket::Core::String>
					("server_host", "localhost").CString();
				string server_port = p_event.GetParameter<Rocket::Core::String>
					("server_port", "1337").CString();
				
				auto sys = static_cast<ClientConnectToServerSystem*>(
					m_world->getSystem(SystemType::ClientConnectoToServerSystem));

				sys->setConnectionAddress(server_address, server_port);
				m_stateComp->setStatesDelta(LOBBY,ENTEREDTHISFRAME);
				m_localState = THISFRAME;
			}
			/*else if(p_value == "join_lobby"){
				m_stateComp->setStatesDelta(LOBBY,ENTEREDTHISFRAME);
				m_localState = THISFRAME;
			}*/
			else if(values[0] == "join_game"){
				m_stateComp->setStatesDelta(INGAME,ENTEREDTHISFRAME);
				m_localState = THISFRAME;
			}
		}
	}
}

void LibRocketEventManagerSystem::processEntities( const vector<Entity*>& p_entities )
{
	//Check if there is a game state entity
	if(p_entities.size()>0){

		//Check if a state has been changed.
		if(m_localState != NONE ){

			switch (m_localState)
			{
			case LibRocketEventManagerSystem::THISFRAME:
				m_localState = VERIFIED;
				break;
			case LibRocketEventManagerSystem::VERIFIED:
				m_localState = PREVIOUSFRAME;
				break;
			case LibRocketEventManagerSystem::PREVIOUSFRAME:
				//Reset the delta to zero
				for (unsigned int i = 0 ; i < EnumGameStates::NUMSTATES; i++){
					m_stateComp->setStatesDelta(static_cast<EnumGameStates>(i),NOTCHANGED);
				}
				m_localState = NONE;
				break;
			}
		}
	}
	if (wantsToExit){
		m_world->requestToShutDown();
	}
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

void LibRocketEventManagerSystem::inserted( Entity* p_entity )
{
	if(m_stateEntity == NULL){
		m_stateEntity = p_entity;
		m_stateComp = static_cast<GameState*>(m_stateEntity->getComponent(
			ComponentType::GameState));
	}
}

void LibRocketEventManagerSystem::removed( Entity* p_entity )
{
	if(m_stateEntity){
		m_stateEntity = NULL;
		m_stateComp = NULL;
	}
}
