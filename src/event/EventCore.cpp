#include "EventCore.hpp"

#include <boost/lexical_cast.hpp>

#include "sbe/event/EventHelper.hpp"

#include <memory>

EventCore* EventCore::Instance;

EventCore::EventCore()
{
	RemoteEventsThisSecond = 0;
	Instance = this;
	idcount = 0;
}

int EventCore::GetEventCount()
{
	int re = RemoteEventsThisSecond;
	RemoteEventsThisSecond = 0;
	return re;
}

// -----------------------------------------------------------------
// Registration and hash stuff
// -----------------------------------------------------------------

bool EventCore::HasEvent(const HashedString::HashType& Hash)
{
	boost::shared_lock<boost::shared_mutex> lock( HashMutex );
	return EventTypes.ElementExists( Hash );
}

bool EventCore::HasEvent(const std::string& Name)
{
	boost::shared_lock<boost::shared_mutex> lock( HashMutex );
	return EventTypes.ElementExists( Name );
}

HashedString::HashType EventCore::GetEventHash(const std::string& Name)
{
	boost::shared_lock<boost::shared_mutex> lock( HashMutex );
	return EventTypes.GetHash( Name );
}

const std::string & EventCore::GetEventName(const HashedString::HashType& Hash)
{
	boost::shared_lock<boost::shared_mutex> lock( HashMutex );
	return EventTypes.GetString( Hash );
}

bool EventCore::RegisterEventName(const std::string& EventName)
{
	boost::lock_guard<boost::shared_mutex> lock( HashMutex );
//	Engine::out() << "[" << __PRETTY_FUNCTION__ << "]: " << "Registered new Event: " << EventName << " - ";

	if (EventTypes.RegisterElement( EventName ))
	{
		//Engine::out() << EventTypes.GetHash( EventName ) << std::endl;
		return true;
	}

	Engine::out(Engine::ERROR) << "[EC] Unable to register new Event!" << std::endl;
	return false;
}



// -----------------------------------------------------------------
// Other stuff
// -----------------------------------------------------------------

void EventCore::Tick()
{
	Event e("EVT_TEMP");

	if (ModulesById.empty())
	{
		Engine::out() << "[EC] No more modules connected, shutting down, " << Module::Get()->GetName() << std::endl;
		Module::Get()->RequestQuit();
	}

//	Engine::out() << "[EC] TICK" << std::endl;

	// iterate all modules
	for ( auto in : ModulesById)
	{

//		Engine::out() << "[EC] Checking Module " << in.second->ID << " [" << in.second->Name << "]" << std::endl;
//		if (in.second->InputQueue.empty())
//			Engine::out() << "[EC] Queue is empty..." << std::endl;

		while (in.second->InputQueue.try_pop(e))
		{
//			Engine::out() << "[EC] Got Event: "<< e.getDebugName() << std::endl;

			for ( auto out : ModulesById)
			{
				if ( in.second->ID != out.second->ID)
				{
//					Engine::out() << " Routing Event '" << e.getDebugName() << "' from '" << in.second->Name << "' to '" << out.second->Name << "'" << std::endl;
					out.second->EvtQ->ThreadSafeQueueEvent(e);
				}
			}
		}
	}



//	for ( size_t id = 0; id < Modules.size(); ++id )
//	{
//		while ( Modules[id]->InputQueue.try_pop(e) )
//		{
//			RouteEvent ( id , e );
//		}
//	}
}

void EventCore::RouteEvent(size_t QueueID,  const Event& e)
{

	ModulesById[QueueID]->EvtQ->ThreadSafeQueueEvent(e);

// TODO: implement routing, for now just send everything
//	for ( size_t QueueID : Routes[ e.getEventType() ] )
//	{
//
//	}
}





// -----------------------------------------------------------------
// Module stuff
// -----------------------------------------------------------------

size_t EventCore::RegisterModule( Module& M )
{
	boost::lock_guard<boost::shared_mutex> lock( QueueMutex );

	size_t newID = idcount++;

	std::shared_ptr<ModuleInfo> m_p( new ModuleInfo( M.GetName(), newID, M.GetEventQueue()) );

	ModulesById[ newID ] 		 = m_p;
	ModulesByName[ M.GetName() ] = m_p;

	return newID;
}

void EventCore::RemoveModule ( size_t ModuleID )
{
	boost::lock_guard<boost::shared_mutex> lock( QueueMutex );

	auto it = ModulesById.find( ModuleID );
	if ( it != ModulesById.end() )
	{
		Engine::out() << "[EC] Module: " << it->second->Name << " - ID: " << it->second->ID << " removed." << std::endl;
		ModulesByName.erase ( it->second->Name );
		ModulesById.erase ( it );
	}
	else
	{
		Engine::out(Engine::ERROR) << "[EC] Can't remove non-registered Module( ID: " << ModuleID << " ) from Eventcore!!" << std::endl;
	}
}

/// Post an event to a specific queue, used by modules to post their Events
void EventCore::PostEventToQueue( size_t ModuleID, const Event& e)
{
	boost::shared_lock<boost::shared_mutex> lock( QueueMutex );

	auto it = ModulesById.find( ModuleID );
	if ( it != ModulesById.end() )
	{
//		Engine::out() << "[EC] Input   Event from Queue " << ModuleID << " [" << Modules[ModuleID]->Name << "]" << std::endl;

		it->second->InputQueue.push ( e );

		RemoteEventsThisSecond++;

//		if ( Modules[ModuleID]->InputQueue.empty() )
//		Engine::out() << "[EC] QUEUE EMPTY AFTER PUSH???" << std::endl;
	}
	else
	{
		Engine::out(Engine::ERROR) << "[EC] PostEvent to invalid Queue " << ModuleID << std::endl;
	}
}

size_t EventCore::GetQueueID( const std::string& Name)
{
	boost::shared_lock<boost::shared_mutex> lock( QueueMutex );

	auto it = ModulesByName.find(Name);
	if ( it == ModulesByName.end() )
	{
		Engine::out(Engine::ERROR) << "[EC] No such Queue with name: " << Name << std::endl;
	}

	return it->second->ID;
}









