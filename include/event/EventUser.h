#ifndef EVTLISTENER_H
#define EVTLISTENER_H

#include "Event.h"

#include "Module.h"

/** An Event user may send and receive Events
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




