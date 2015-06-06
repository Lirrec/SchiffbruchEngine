#ifndef EVTLISTENER_H
#define EVTLISTENER_H

#include "sbe/event/Event.hpp"

#include "sbe/Module.hpp"

#include <map>

namespace sbe
{


	/**
		Base class for all classes which receive events.
		An Event user may receive Events (and send) Events.
		A unique EventUser-id is generated for each Eventuser.

		To receive Events call RegisterForEvent() with the desired Event and a callback
		or implement the virtual HandleEvent() function.

	*/
	class EventUser
	{
	public:
		EventUser();

		virtual ~EventUser();

		typedef std::function<void(Event& e)> EventHandler;

		/**
			Register for an Event by Type.
			@param EvtType the Event type
			@param priority the priority of this Eventhandler, higher means being called before all the low priority handlers
		*/
		void RegisterForEvent(const Event::EventType& EvtType, int priority = 0);

		/**
			Register for an Event by Name.
			@param EvtName the Event name
			@param priority the priority of this Eventhandler, higher means being called before all the low priority handlers
		*/
		void RegisterForEvent(const std::string& EvtName, int priority = 0);

		/**
			Register a callback for an Event by name.
			This will replace any existing callbacks.
			@param EvtName the Event name
			@param Handler a callback which is called with the event as first parameter if a event with EvtName occurs.
			@param priority the priority of this Eventhandler, higher means being called before all the low priority handlers
		*/
		void RegisterForEvent(const std::string& EvtName, const EventHandler& Handler, int priority = 0);

		/**
			Register a callback for an Event by EventType.
			This will replace any existing callbacks.
			@param EvtName the Event name
			@param Handler a callback which is called with the event as first parameter if a event with EvtName occurs.
			@param priority the priority of this Eventhandler, higher means being called before all the low priority handlers
		*/
		void RegisterForEvent(const Event::EventType& EvtType, const EventHandler& Handler, int priority = 0);

		/**
			Remove this listener from all registered Events
		*/
		void UnregisterThis();

	protected:

		/**
			Handle Events manually.
			This function will be called with each event that is not already handeled by a callback.
		*/
		virtual void HandleEvent(Event&) { };

	private:
		friend class EventQueue;

		void RealHandleEvent(Event& e);

		std::map<Event::EventType, EventHandler> Callbacks;


	};
} // namespace sbe
#endif // EVTLISTENER_H
