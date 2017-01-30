#ifndef EVENT_H
#define EVENT_H

#include <boost/uuid/uuid.hpp>
#include <boost/any.hpp>

#include <sbe/util/HashedString.hpp>
#include <sbe/Engine.hpp>

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
			- EventUser: If some class wants to be able to receive Events it has to inherit from EventUser
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

	 		@see Module::QueueEvent()

			Example:
			@code

			// simple Event without data
			Module::Get()->QueueEvent("MY_EVT", true); // set 'global' parameter to false to only send to own module

	 		// simple Event with data
			float mydata = 5;
			Module::Get()->QueueEvent("MY_EVT", mydata, true); // set 'global' parameter to false to only send to own module

	 		// simple Event with multiple data ( note that the 'global' parameter is now second and all other parameters are data
			float myfloat = 5;
			int myint = 2;
			myClass mC;
			// this will send an Event with Name "MY_EVT".
			// the data will be of type std::tuple<float, int, myClass>
			Module::Get()->QueueEvent("MY_EVT", true, myfloat, myint, mC); // set 'global' parameter to false to only send to own module

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

			});

	 		// if the event has data use the following version
	 		// this also works with multiple ( currently up to 4 ) parameters
	 		RegisterForEvent<int>("MY_EVT", [](int i) {
	 			// your handling code here
	 		});

			// member function callback, this requires an "Event Definiton" which defines the required parameters
			// this example is taken from sbe::Renderer::addActor / sbe::Renderer::addActorEvent

			// addActorEvent is implemented like this, and creates an EventDef:
			// return sbe::makeEventDef<hash_name("ADD_ACTOR")>(&Renderer::addActor);
			// then call with 'this' and the EventDef
			RegisterMemberAsEventCallback(this, addActorEvent());




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
			Once an Event occurs for which the EventUser is registered either the callback given as parameter to RegisterForEvent is called,
			or the HandleEvent() method will be called with the corresponding Event.




			Some Event Types and their usage (incomplete):

			VIEW_DBG_STRING(string, string) - intended for sending some data each frame to display on screen, eg. fps, events per seconds etc.

			For an overview of the SFML Eventsystem see the according <a href="http://sfml-dev.org/">documentation</a>.

		The second event system are the events supplied by SFML for receiving mostly hardware events about input.
	*/



	class Event
	{
	public:
		typedef boost::any EventData;
		typedef HashType EventType;

		/**
			Constructor. Takes the Name of the Event as String
		*/
		Event(const std::string& EventName);

		///	Constructor. Takes the Hash of the Event
		Event(const EventType& Type);

		///Constructor. Takes the Hash of the Event
		Event(const EventType& Type, const boost::any& _Data);

		/**
			Constructor. Takes the Name of the Event as String and a boost::any as data
		*/
		Event(const std::string& EventName, const boost::any& _Data);

		/** returns the Eventtype( = Hash) of a given eventname ( string ) */
		static Event::EventType hashName(const std::string& EventName);

		/**
			Constructor. Takes the Name of the Event as String and anything as data
		*/
		template<class T>
		Event(const std::string& EventName, const T& _Data) {
			init(EventName, boost::any(_Data));
		}


		/// Sets an arbitrary Type as data
		template<class T>
		void SetData(const T& _Data) {
			EvtData = _Data;
		}

		/// returns the data stored within the event
		EventData& Data() { return EvtData; };
		const EventData& cData() const { return EvtData; };

		/// returns the type of this event
		EventType getEventType() const { return Type; };

		/// returns the eventname as string
		std::string getDebugName();

		/// check if the event is of the given type (string-name)
		bool Is(const std::string& Name) const;

		/// check if the event is of the given string-name and the data is of the given type
		bool Is(const std::string& Name, const std::type_info& type) const;

		/// check if the event is of the given type (EventType / string-hash)
		bool Is(const EventType& otherType) const { return Type == otherType; };

	private:

		void init(const std::string& EventName, const boost::any& _data);

		EventType Type;
		EventData EvtData;
	};

} // sbe

typedef sbe::Event Event;

#endif // EVENT_H
