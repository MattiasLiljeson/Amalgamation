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
//#include <Shell.h>
#include "EventHandler.h"
#include "EventInstancer.h"
//#include "GameDetails.h"
#include <map>
#include <Globals.h>
#include <ToString.h>
#include <DebugUtil.h>
#include "libRocketBackendSystem.h"

LibRocketEventManagerSystem::LibRocketEventManagerSystem()
	: EntitySystem(SystemType::LibRocketEventManagerSystem)
{
	context = NULL;
	wantsToExit = false;
	event_handler = NULL;
}

LibRocketEventManagerSystem::~LibRocketEventManagerSystem()
{
	Shutdown();
}

void LibRocketEventManagerSystem::initialize()
{
	auto rocketBackend = static_cast<LibRocketBackendSystem*>(
		m_world->getSystem(SystemType::LibRocketBackendSystem));

	context = rocketBackend->getContext();

	EventInstancer* eventInstancer = new EventInstancer(this);
	Rocket::Core::Factory::RegisterEventListenerInstancer(eventInstancer);
	eventInstancer->RemoveReference();
}

// Releases all event handlers registered with the manager.
void LibRocketEventManagerSystem::Shutdown()
{
	//for (EventHandlerMap::iterator i = event_handlers.begin(); i != event_handlers.end(); ++i)
	//	delete (*i).second;

	event_handlers.clear();
	event_handler = NULL;
}

// Registers a new event handler with the manager.
void LibRocketEventManagerSystem::registerEventHandler(const Rocket::Core::String& handler_name, EventHandler* handler)
{
	//handler->ConnectToManager(this);
	// Release any handler bound under the same name.
	EventHandlerMap::iterator iterator = event_handlers.find(handler_name);
	if (iterator != event_handlers.end())
	{	//delete (*iterator).second;
		DEBUGWARNING(((
			toString("LibRocketEventManagerSystem::registerEventHandler\nAttempting to register EventHandler[")
			+ toString(handler_name.CString())
			+ toString("] which has already been registred in the EventManager.")).c_str()));
	}
	event_handlers[handler_name] = handler;
}

EventHandler* LibRocketEventManagerSystem::UnregisterEventHandler( const Rocket::Core::String& handler_name )
{
	EventHandlerMap::iterator iterator = event_handlers.find(handler_name);
	EventHandler* handler = NULL;
	if (iterator != event_handlers.end())
	{
		handler = (*iterator).second;
		handler->connectToManager(NULL);
		event_handlers[handler_name] = NULL;
	}
	return handler;
}

// Processes an event coming through from Rocket.
void LibRocketEventManagerSystem::processEvent(Rocket::Core::Event& event, const Rocket::Core::String& value)
{
	Rocket::Core::StringList commands;
	Rocket::Core::StringUtilities::ExpandString(commands, value, ';');
	for (size_t i = 0; i < commands.size(); ++i)
	{
		// Check for a generic 'load', 'exit' or 'set' command.
		Rocket::Core::StringList values;
		Rocket::Core::StringUtilities::ExpandString(values, commands[i], ' ');

		if (values.empty())
			return;

		auto ownerDocument = event.GetTargetElement()->GetOwnerDocument();
		if (values[0] == "modal" && !ownerDocument->IsModal())
		{
			ownerDocument->Show(Rocket::Core::ElementDocument::MODAL);
		}
		else if (values[0] == "goto" && values.size() > 1)
		{
			// Load the window, and if successful hide the old window.
			if (LoadWindow(values[1]))
			{
				event.GetTargetElement()->GetOwnerDocument()->Hide();
			}
		}
		else if (values[0] == "load" &&	values.size() > 1)
		{
			// Load the window.
			LoadWindow(values[1]);
		}
		else if (values[0] == "close")
		{
			Rocket::Core::ElementDocument* target_document = NULL;

			if (values.size() > 1)
				target_document = context->LoadDocument(values[1].CString());
			else
				target_document = event.GetTargetElement()->GetOwnerDocument();

			if (target_document != NULL)
				target_document->Close();
		}
		else if (values[0] == "exit")
		{
			wantsToExit = true;
		}
		else if (values[0] == "pause")
		{
			//GameDetails::SetPaused(true);
		}
		else if (values[0] == "unpause")
		{
			//GameDetails::SetPaused(false);
		}
		else
		{
			if (event_handler != NULL)
				event_handler->processEvent(event, commands[i]);
		}
	}
}

// Loads a window and binds the event handler for it.
bool LibRocketEventManagerSystem::LoadWindow(const Rocket::Core::String& window_name)
{
	// Set the event handler for the new screen, if one has been registered.
	EventHandler* old_event_handler = event_handler;
	EventHandlerMap::iterator iterator = event_handlers.find(window_name);
	if (iterator != event_handlers.end())
		event_handler = (*iterator).second;
	else
		event_handler = NULL;

	// Attempt to load the referenced RML document.
	/*Rocket::Core::String document_path = 
		Rocket::Core::String((GUI_MENU_PATH + toString("assets/")).c_str())+
		window_name + Rocket::Core::String(".rml");
	*/
	//Rocket::Core::String document_path = 
	//	(GUI_MENU_PATH + 
	//	toString("temp/") + 
	//	toString((window_name).CString()) +
	//	toString(".rml")).c_str();
	
	//Rocket::Core::ElementDocument* document = context->LoadDocument(document_path.CString());
	auto document = context->GetDocument(window_name);
	if (document == NULL)
	{
		DEBUGWARNING(((
			toString("LibRocketEventManagerSystem::loadWindow\nNo document with the body id\"")
			+ toString(window_name.CString())
			+ toString("\" has been loaded.")).c_str()));
		event_handler = old_event_handler;
		return false;
	}

	document->Show();
	context->PullDocumentToFront(document);

	// Remove the caller's reference.
	//document->RemoveReference();

	return true;
}

void LibRocketEventManagerSystem::registerEventHandler( EventHandler* handler )
{
	registerEventHandler(handler->getName().c_str(), handler);
}

void LibRocketEventManagerSystem::process()
{
	if (wantsToExit)
		m_world->requestToShutDown();
}
