#ifndef EVENT_H
#define EVENT_H




#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/any.hpp>

#include "HashedString.h"
#include "Geom.h"

#include "Engine.h"

#include <map>
#include <vector>
#include <list>


/**
	Event System overview:
	Schiffbruchagain uses 2 kinds of Events.
	The first one is the eventsystem supplied by SFML for receiving hardwareevents about input.
	The second one is a very small custom Eventsystem for sending game specific events.


	TODO/PLANNED:

	Make it possible to use events in multiple Threads.
	This would need a seperate Eventmanger for each thread,
	synchronized with the other eventmanagers or the main evtmgr

	Some Event Types and their usage:


	EVT_TICK
	EVT_FRAME
	EVT_ANIM_END
	EVT_GUY_FINISH_MOVE
	EVT_GAME_ABORT

Debug:
	The Debugwindow screen element listens on VIEW_DBG_STRING(string, string) events
	where the first string is the name of the property and the last string the value
	(which can be updated as desired)


LOGIC -> View Connection

Grep ( Graphical Re






*/

class Event
{
	public:

		Event( const std::string& EventName, const boost::uuids::uuid& Source = invalid_source_id );

		typedef boost::any EventData;
		typedef HashedString::HashType EventType;

		static const  boost::uuids::uuid invalid_source_id;

		/// Sets an arbitrary Type as data
		template < class T >
		void SetData( const T& Data )
		{
			EvtData = Data;
		}

		/// returns the data stored within the event
		EventData& Data() { return EvtData; };

		/// returns the type of this event
		EventType getEventType() { return Type; };

		std::string getDebugName();

		bool Is(const std::string& Name);
		bool Is(const EventType& otherType) { return Type == otherType; };

	private:

		EventType Type;
		boost::uuids::uuid SourceID;
		boost::uuids::uuid EventID;
		EventData EvtData;
};

#endif // EVENT_H

