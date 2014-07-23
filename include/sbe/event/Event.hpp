#ifndef EVENT_H
#define EVENT_H

#include <boost/uuid/uuid.hpp>
#include <boost/any.hpp>

#include "sbe/util/HashedString.hpp"
#include "sbe/Engine.hpp"

#include <map>
#include <vector>
#include <list>



namespace sbe
{


	/**
		Event System overview:
		SchiffbruchEngine uses 2 kinds of Events.
		The first one is a custom Eventsystem for sending game specific events:

			The EventSystem consists of the following Classes:
			- Event: An individual event, Events are defined by their name and contain eventdata
			- EventUser: If some class wants to be able to receive Events it has to inherit EventUser
			- SFMLEventConverter: This class allows to wrap any sfml events received into normal events and thus handle them the same way




			All event names should start with "EVT_" and contain uppercase letters, underscores and numbers (although the implementation allows arbitrary strings)
			Event data:
			Events may store data of an arbitrary type, using boost::any.
			This allows for a great flexibility but also forces users of eventdata to check for the correct data-type before using data and then casting to the required datatype using boost::any_cast
			An example may be found in the Receiving Events section.


			Sending Events:
			Sending Events works by adding the Event to send to your current Modules EventQueue. This is done by Module::QueueEvent() and Module::PostEvent().
			QueueEvent will send the event the next time the eventqueue is checked, the second parameter will also send the event to other modules' eventqueues.
			PostEvent will immediatly send the event to all registered listeners in the local module, thus bypassing the normal eventqueue.

			Example:
			@code
			// event without data
			Event e("EVT_MY_EVT");
			// event with data ( templated, pass anything you want )
			float mydata = 5;
			Event d("EVT_MY_DATA", mydata);
			// another way to set the data:
			d.SetData( mydata );

			Module::Get()->QueueEvent(e, true); // set to false to only send to own module
			@endcode

			The use of Modules (@see Module) makes it possible to use events in multiple Threads.
			Events can either be sent locally, meaning they are only received in the current Module/Thread, or global,
			which will sent them to all Modules. This allows modules to use "internal" events which are not visible to other modules.
			Events are by default sent local, to send a global event set the second parameter of Module::QueueEvent() to true.
			Module::PostEvent() doesn't support sending events globally.

			Example:
			@code

			// the simple version with lambda callback
			RegisterForEvent("EVT_MY_EVT1", [](Event& e) {
								// your handling code here
			});

			// member function callback,
			// you have to use two lines so the  compiler will be able to
			// deduct the correct types ( at least on gcc 4.8 a single line won't work )
			Eventhandler e = std::bind( &MyClass::member1, this );
			RegisterForEvent("EVT_MY_EVT2", e );

			// the verbose way, implement the HandleEvent method.
			// somewhere in your class' code (constructor/ init)
			RegisterForEvent("EVT_MY_EVT");


			// implement the HandleEvent function
			myClass::HandleEvent( Event& e )
			{
				// check event type and data ( second parameter is optional )
				if (e.Is("EVT_MY_EVT", typeid( float)))
				{
					// cast into desired type
					float D = boost::any_cast<float>(e.Data());
				}
			}
			@endcode

			Receiving Events:
			To receive events a class has to inherit EventUser,
			Then register for the Events it is interested in by calling this->RegisterForEvent() ( inherited from EventUser::RegisterForEvent() )
			Once an Event occurs for which the EventUser is registered either the callback given as parameter to RegisterForEvent is called or the HandleEvent() method will be called with the corresponding Event.




			Some Event Types and their usage (incomplete):

			VIEW_DBG_STRING(string, string) - intended for sending some data each frame to display on screen, eg. fps, events per seconds etc.

			For an overview of the SFML Eventsystem see the according <a href="http://sfml-dev.org/">documentation</a>.

		The second one is the eventsystem supplied by SFML for receiving mostly hardware events about input.
	*/



	class Event
	{
		public:
			typedef boost::any EventData;
			typedef HashedString::HashType EventType;

			/**
				Constructor. Takes the Name of the Event as String
			*/
			Event( const std::string& EventName);
			/**
				Constructor. Takes the Name of the Event as String and a boost::any as data
			*/
			Event( const std::string& EventName, const boost::any& _Data );

			/** returns the Eventtype( = Hash) of a given eventname ( string ) */
			static Event::EventType hashName( const std::string& EventName);

			/**
				Constructor. Takes the Name of the Event as String and anything as data
			*/
			template < class T >
			Event( const std::string& EventName, const T& _Data )
			{
				init( EventName, boost::any(_Data) );
			}



			/// Sets an arbitrary Type as data
			template < class T >
			void SetData( const T& _Data )
			{
				EvtData = _Data;
			}

			/// returns the data stored within the event
			EventData& Data() { return EvtData; };

			/// returns the type of this event
			EventType getEventType() const { return Type; };

			/// returns the eventname as string
			std::string getDebugName();

			/// check if the event is of the given type (string-name)
			bool Is(const std::string& Name) const;

			/// check if the event is of the given string-name and the data is of the given type
			bool Is(const std::string& Name, const std::type_info &type ) const ;

			/// check if the event is of the given type (EventType / string-hash)
			bool Is(const EventType& otherType) const { return Type == otherType; };

		private:

			void init( const std::string& EventName, const boost::any& _data);

			EventType Type;
			EventData EvtData;
	};

} // sbe

typedef sbe::Event Event;

#endif // EVENT_H
