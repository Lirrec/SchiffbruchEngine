#ifndef EVTLISTENER_H
#define EVTLISTENER_H

#include "sbe/event/Event.hpp"

#include "sbe/Module.hpp"

/**
	Base class for all classes which receive events.
	An Event user may receive Events (and send) Events.
	A unique EventUser-id is generated for each Eventuser.

	To receive Events call RegisterForEvent() with the desired Event
	and implement the virtual HandleEvent() function.

*/
class EventUser
{
	public:
		EventUser();
		virtual ~EventUser();

		/// Handle an event
		virtual void HandleEvent( Event& ) {};

		const boost::uuids::uuid& GetID() { return ID; };

	protected:
		boost::uuids::uuid ID;

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
		void RegisterForEvent(const std::string& EvtName , int priority = 0);

		/**
			Remove this listener from all registered Events
		*/
		void UnregisterThis();


};

#endif // EVTLISTENER_H




