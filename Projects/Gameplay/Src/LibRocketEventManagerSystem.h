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
#include <Rocket/Core/Event.h>
#include <EventManager.h>
#include <EntitySystem.h>
#include <stack>

namespace Rocket
{
	namespace Core
	{
		class Context;
	}
}
class EventHandler;
class GameState;

/**
	@author		Peter Curry
	@modifiers	Robin Thunström, Alexander Brodén
 */

class LibRocketEventManagerSystem : public EntitySystem, public EventManager
{
public:
	LibRocketEventManagerSystem();
	~LibRocketEventManagerSystem();

	void initialize();
	/// Releases all event handlers registered with the manager.
	void shutdown();

	void processEntities(const vector<Entity*>& p_entities);

	void registerEventHandler(EventHandler* p_handler);

	EventHandler* unregisterEventHandler(const Rocket::Core::String& p_handlerName);

	void clearDocumentStack();

	/// Processes an event coming through from Rocket.
	/// @param[in] event The Rocket event that spawned the application event.
	/// @param[in] value The application-specific event value.
	void processEvent(Rocket::Core::Event& p_event, const Rocket::Core::String& p_value);
	/// Loads a window and binds the event handler for it.
	/// @param[in] p_windowName The name of the window to load.
	bool loadWindow(const Rocket::Core::String& p_windowName);
public:
	bool wantsToExit;
private:
	/// Registers a new event handler with the manager.
	/// @param[in] p_handlerName The name of the handler; this must be the same as the 
	/// window it is handling events for.
	/// @param[in] handler The event handler.
	void registerEventHandler(const Rocket::Core::String& p_handlerName, EventHandler* p_handler);

	/// Pops all docIds and hides them until the parameter doc id is the top one.
	void clearStackUntilFoundDocId(const Rocket::Core::String&  p_docId);

	virtual void inserted( Entity* p_entity );

	virtual void removed( Entity* p_entity );


private:
	// The game's element context (retrieved from the rocket backend system).
	Rocket::Core::Context* m_context;
	// The event handler for the current screen. This may be NULL if the current screen 
	// has no specific functionality.
	EventHandler* m_eventHandler;
	// The event handlers registered with the manager.
	typedef std::map< Rocket::Core::String, EventHandler* > EventHandlerMap;
	EventHandlerMap m_eventHandlers;

	Rocket::Core::String				m_currentDocId;
	std::stack<Rocket::Core::String>	m_docIdStack;
	Entity*								m_stateEntity;
	GameState*							m_stateComp;
	int m_stateDelay;
};


