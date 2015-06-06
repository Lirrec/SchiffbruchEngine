#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <SFML/System/NonCopyable.hpp>

#include "sbe/util/ConcurrentQueue.hpp"
#include "sbe/event/Event.hpp"


namespace sbe
{

	class EventUser;

	/** A lokal EventQueue, one of those exists per thread
	*/
	class EventQueue : sf::NonCopyable
	{
	public:

		/// this allows the Eventlisteners only to register to the eventmanger as the required methods are private
		friend class EventUser;

		EventQueue();

		/// send Event-Queues. Called by Module main loop
		void Tick();

		/// Post an Event
		void PostEvent(Event& e);

		/// Queue an Event.
		/// will be fired on next tick
		/// @param e event to put into the queue
		/// @param global determines if the Event will be sent to all modules or only visible in this module
		/// or only fired in this thread
		void QueueEvent(const Event& e, bool global = false);

		/// Same as above, but event will be constructed in place
		void QueueEvent(const std::string& Name, bool global = false);

		/// Queue an Event from another Thread
		void ThreadSafeQueueEvent(const Event& e);

		/// Adds a EventUser to the users list
		void AddEventUser(EventUser* User);

		/// Removes a listener from the game event queue
		void RemoveEventUser(EventUser* Listener);

		/// returns the number of sent local events and resets those to 0
		int GetEventCount();

	private:

		int LocalEventsThisSecond;

		/**
			Registers a listener for game events.
			@param priority Listeners with lower priority will be called before elements with higher priority ( = internal ordering of std::multimap), order of callback on same priority is undefined
		*/
		void RegisterEventUser(EventUser* Listener, Event::EventType EvtType, int priority = 0);

		void RegisterEventUser(EventUser* Listener, const std::string& EvtName, int priority = 0);

		//typedef std::map< boost::uuids::uuid, EventUser*>						EventUserMap;
		typedef std::map<Event::EventType, std::multimap<int, EventUser* >> EventListenerMap;

		//EventUserMap 			EventUsers;
		EventListenerMap EventListeners;

		std::deque<Event> LocalEventQueue;
		ConcurrentQueue<Event> ThreadedEventQueue;
	};

} // namespace sbe
#endif // EVENT_QUEUE_H
