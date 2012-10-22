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
	SchiffbruchEngine uses 2 kinds of Events.
	The first one is the eventsystem supplied by SFML for receiving mostly hardware events about input.
	The second one is a custom Eventsystem for sending game specific events.

	For an overview of the SFML Eventsystem see the according <a href="http://sfml-dev.org/">documentation</a>.

	The EventSystem consists of the following Classes:
	- Event: An individual event, Events are defined by their name and contain eventdata


	The use of Modules (@see Module) makes it possible to use events in multiple Threads.
	Events can either be sent locally, meaning they are only received in the current Module/Thread, or global,
	which will sent them to all Modules.
	This allows modules to use "internal" events which are not visible to other modules.


	All event names should start with "EVT_" and contain uppercase letters, underscores and numbers (although the implementation allows arbitrary strings)

	Some Event Types and their usage (incomplete):

	VIEW_DBG_STRING(string, string) - intended for sending some data each frame to display on screen, eg. fps, events per seconds etc.


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

