#ifndef EVTLISTENER_H
#define EVTLISTENER_H

#include "Event.h"

#include "Module.h"

/** Base class for all classes which receive events.
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

		void RegisterForEvent(const Event::EventType& EvtType, int priority = 0);
		void RegisterForEvent(const std::string& EvtName , int priority = 0);
		void UnregisterThis();


};

#endif // EVTLISTENER_H




