#include "EventQueue.hpp"

#include <boost/lexical_cast.hpp>

#include "EventCore.hpp"
#include "sbe/event/EventHelper.hpp"

#include "../modules/Core.hpp"

#include <list>

namespace sbe
{
	EventQueue::EventQueue()
	{
		LocalEventsThisSecond = 0;


	}

	// -----------------------------------------------------------------
	// Other stuff
	// -----------------------------------------------------------------

	void EventQueue::Tick()
	{
		// create temporary lists
		std::list<Event> tmp_EventQueue = LocalEventQueue;

		LocalEventQueue.clear();

		// Add the Events from other Threads to the EventQueue
		Event e("EVT_TICK");
		while (ThreadedEventQueue.try_pop(e) )
		{
			//Engine::out() << "[FE:"<< Module::MyName() <<"] Found Remote Event: " << e.getDebugName() << std::endl;
			tmp_EventQueue.push_front( e );

			// exclude remote origin events
			LocalEventsThisSecond--;
		}

		for ( auto event : tmp_EventQueue)
		{
			LocalEventsThisSecond++;
			PostEvent(event);
		}

	}

	int EventQueue::GetEventCount()
	{
		int re = LocalEventsThisSecond;
		LocalEventsThisSecond = 0;
		return re;
	}

	void EventQueue::RegisterEventUser(EventUser* Listener, const std::string& EvtName, int priority)
	{
		if ( !Core::EvtCore->HasEvent( EvtName ) )
		{
			//Engine::out() << "[EventQueue]: " << "Listener " << Listener << " registered for new Event '" << EvtName << "'" << std::endl;
			Core::EvtCore->RegisterEventName( EvtName );
		}

		RegisterEventUser( Listener, Core::EvtCore->GetEventHash( EvtName), priority);
	}

	void EventQueue::RegisterEventUser ( EventUser* Listener, Event::EventType EvtType, int priority )
	{
		if (!Core::EvtCore->HasEvent( EvtType ) )
		{
			//Engine::out() << "[EventQueue]: " << "Listener " << Listener << " tried to register for non-existing EventType " << EvtType << std::endl;
			return;
		}

		if ( Listener != 0 )
		{
			EventListeners[EvtType].insert ( std::make_pair ( priority, Listener ) );
			EventUsers[Listener->GetID()] = Listener;

			Engine::out(Engine::SPAM) << "[EventQueue] New listener: " << Core::EvtCore->GetEventName( EvtType) << std::endl;
		}
	}



	void EventQueue::PostEvent ( Event& e )
	{

		//if (!e.Is("EVT_FRAME")) Engine::out() << "[FE:"<< Module::MyName() <<"] " << e.getDebugName() << std::endl;


		for ( auto & EvL : EventListeners[ e.getEventType() ] )
		{
			EvL.second->HandleEvent ( e );
		}
	}

	void EventQueue::QueueEvent(const std::string & Name, bool global)
	{
		Event e(Name);

		if (global)
		{
			Core::EvtCore->PostEventToQueue( Module::Get()->GetQueueID(), e );
		}

		//Engine::out() << "Pushing event " << e.getDebugName() << std::endl;
		LocalEventQueue.push_back(e);
	}

	void EventQueue::QueueEvent( const Event& e, bool global)
	{
		if (global)
		{
			Core::EvtCore->PostEventToQueue( Module::Get()->GetQueueID(), e );
		}

		//Engine::out() << "Pushing event " << e.getDebugName() << std::endl;
		LocalEventQueue.push_back(e);
	}

	void EventQueue::ThreadSafeQueueEvent(const Event& e)
	{
		ThreadedEventQueue.push(e);
	}

	void EventQueue::AddEventUser( EventUser* User )
	{
		if (User == nullptr )
		{
			Engine::out(Engine::ERROR) << "[EventQueue:AddEventUser] Got nullptr!" << std::endl;
			return;
		}

		EventUsers[User->GetID()] = User;
	}

	void EventQueue::RemoveEventUser ( EventUser* Listener )
	{
		if (Listener == nullptr )
		{
			Engine::out(Engine::ERROR) << "[EventQueue:RemoveEventUser] Got nullptr!" << std::endl;
			return;
		}

		// Hes probably only inthe EventUsers list, no listener
		EventUsers.erase (Listener->GetID());

		for ( auto & EventVec : EventListeners )
		{
			//Engine::out() << "Size: " << EventVec.second.size() << std::endl;
			for (auto it = EventVec.second.begin(); it != EventVec.second.end(); /*empty*/)
			{
				if (it->second == Listener) { EventVec.second.erase( it++ ); }
				else { ++it; }
			}
		}
	}
} // namespace sbe
