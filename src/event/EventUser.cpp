#include "sbe/event/EventUser.hpp"

#include "sbe/Engine.hpp"
#include "EventCore.hpp"

#include "../modules/Core.hpp"

namespace sbe
{
	// EventUser
	EventUser::EventUser()
	{
		Module::Get()->GetEventQueue()->AddEventUser( this );
	}

	EventUser::~EventUser()
	{
		if (Module::Get())
			Module::Get()->GetEventQueue()->RemoveEventUser( this );
	}

	void EventUser::RegisterForEvent(const Event::EventType& EvtType, int priority)
	{
		if (Core::EvtCore->HasEvent( EvtType ))
		{
			Module::Get()->GetEventQueue()->RegisterEventUser(this, EvtType, priority);
		}
		else
		{
			Engine::out(Engine::ERROR) << "[EventUser] Couldn't register for non-existing Event " << EvtType << std::endl;
		}
	}

	void EventUser::RegisterForEvent(const std::string& EvtName, int priority)
	{
		Module::Get()->GetEventQueue()->RegisterEventUser(this, EvtName, priority);
	}
	void EventUser::RegisterForEventS(const std::string& EvtName)
	{
		Module::Get()->GetEventQueue()->RegisterEventUser(this, EvtName, 0);
	}

	void EventUser::UnregisterThis()
	{
		// not implemented
	}
} // namespace sbe
