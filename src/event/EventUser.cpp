#include "EventUser.h"

#include "Engine.h"
#include "EventCore.h"

// EventUser
EventUser::EventUser() : ID(Module::NewUUID())
{
	Module::Get()->GetEventQueue()->AddEventUser( this );
};

EventUser::~EventUser()
{
	Module::Get()->GetEventQueue()->RemoveEventUser( this );
};

void EventUser::RegisterForEvent(const Event::EventType& EvtType, int priority)
{
	if (Core::EvtCore->HasEvent( EvtType ))
	{
		Module::Get()->GetEventQueue()->RegisterEventUser(this, EvtType, priority);
	}
	else
	{
		Engine::out() << "ERROR: Couldn't register for non-existing Event " << EvtType << std::endl;
	}
}

/** @brief RegisterForEvent
  *
  * @todo: document this function
  */
void EventUser::RegisterForEvent(const std::string& EvtName, int priority)
{
	Module::Get()->GetEventQueue()->RegisterEventUser(this, EvtName, priority);
}
