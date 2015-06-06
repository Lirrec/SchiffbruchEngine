#ifndef EVTMGR_H
#define EVTMGR_H

#include <boost/thread.hpp>

#include "sbe/Module.hpp"
#include "sbe/event/Event.hpp"
#include "sbe/util/ConcurrentQueue.hpp"
#include "sbe/util/HashedString.hpp"
#include "sbe/util/HashManager.hpp"

//internal headers
#include "EventQueue.hpp"

#include <forward_list>
#include <map>
#include <string>
#include <vector>
#include <tuple>

namespace sbe
{
	using std::size_t;

	/** Handles Input
	*/
	class EventCore : sf::NonCopyable
	{
	public:

		/// this allows the EventQueues only to register to the eventcore as the required methods are private
		friend class EventQueue;

		EventCore();

		/// returns the global Eventcore Singleton
		static EventCore* getInstance() { return Instance; };

		// ---------------- Event Management - Thread Safe -----------

		/// Register a new event
		/// @return false if this eventtype already exists or there was an hashcollision with an existing eventType
		bool RegisterEventName(const std::string& EventName);

		/// returns true if this type of event is registered ( by name )
		bool HasEvent(const std::string& Name);

		/// returns true if this type of event is registered ( by hash )
		bool HasEvent(const HashedString::HashType& Hash);

		const std::string& GetEventName(const HashedString::HashType& Hash);

		HashedString::HashType GetEventHash(const std::string& Name);



		// -------------- Module/EventQueue Managment --------------------

		/// register another Module.
		/// @return returns the modules QueueID
		size_t RegisterModule(Module& M);

		/// unregister a Module, EventCore shuts down if there are no more modules connected
		void RemoveModule(size_t ModuleID);

		/// Post an event to a specific queue, used by modules to post their Events
		void PostEventToQueue(size_t ModuleID, const Event& e);

		/// send Event-Queues. Called by Game main loop
		void Tick();

		/// returns the number of sent local events and resets those to 0
		int GetEventCount();

	private:

		class ModuleInfo
		{
		public:
			ModuleInfo(const ModuleInfo& rhs)
					: Name(rhs.Name), ID(rhs.ID), EvtQ(rhs.EvtQ), InputQueue() {
				//Engine::out() << "MInfo copy-constructed" << std::endl;
			}

			ModuleInfo& operator=(const ModuleInfo& rhs) {
				//Engine::out() << "MInfo copied" << std::endl;
				Name = rhs.Name;
				ID = rhs.ID;
				EvtQ = rhs.EvtQ;
				//InputQueue = ConcurrentQueue<Event>();
				return *this;
			}

			ModuleInfo(const std::string& name, size_t _ID, EventQueue* _EvtQ)
					: Name(name), ID(_ID), EvtQ(_EvtQ), InputQueue() { }

			std::string Name;
			size_t ID;
			EventQueue* EvtQ;
			ConcurrentQueue<Event> InputQueue;
		};

		size_t GetQueueID(const std::string& Name);

		void RouteEvent(size_t QueueID, const Event& e);

		size_t idcount;
		bool justStarted;

		boost::shared_mutex HashMutex;
		boost::shared_mutex QueueMutex;

		static EventCore* Instance;

		int RemoteEventsThisSecond;

		/// stores the modules by name
		std::map<std::string, std::shared_ptr<ModuleInfo> > ModulesByName;
		/// stores the modules by id
		std::map<size_t, std::shared_ptr<ModuleInfo> > ModulesById;

		/// a list of eventhashes and the route
		//std::map< Event::EventType, std::forward_list<size_t> > Routes;

		HashManager EventTypes;
	};

} // namespace sbe
#endif // EVTMGR_H
