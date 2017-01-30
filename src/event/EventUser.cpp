#include <sbe/event/EventUser.hpp>

#include "EventCore.hpp"


#include <sbe/Module.hpp>

namespace sbe
{
	// EventUser
	EventUser::EventUser() {
		Module::Get()->GetEventQueue()->AddEventUser(this);
	}

	EventUser::~EventUser() {
		if (Module::Get())
			Module::Get()->GetEventQueue()->RemoveEventUser(this);
	}

	void EventUser::RegisterForEvent(const Event::EventType& EvtType, int priority) {
		if (EventCore::getInstance()->HasEvent(EvtType))
		{
			Module::Get()->GetEventQueue()->RegisterEventUser(this, EvtType, priority);
		}
		else
		{
			//Engine::out(Engine::ERROR) << "[EventUser] Couldn't register for non-existing Event " << EvtType << std::endl;
			Engine::out(Engine::ERROR) << "[EventUser] Registering for unknown Event " << EvtType << std::endl;
			Module::Get()->GetEventQueue()->RegisterEventUser(this, EvtType, priority);
		}
	}

	void EventUser::RegisterForEvent(const std::string& EvtName, int priority) {
		Module::Get()->GetEventQueue()->RegisterEventUser(this, EvtName, priority);
	}

	void EventUser::RegisterForEvent(const std::string& EvtName, const EventHandler& Handler, int priority) {
		RegisterForEvent(EvtName, priority);
		Callbacks[Event::hashName(EvtName)] = Handler;
	}

	void EventUser::RegisterForEvent(const Event::EventType& EvtType, const EventHandler& Handler, int priority) {
		RegisterForEvent(EvtType, priority);
		Callbacks[EvtType] = Handler;
	}

	void EventUser::RealHandleEvent(Event& e) {
		auto it = Callbacks.find(e.getEventType());
		if (it != Callbacks.end())
			(it->second)(e);
		else
			HandleEvent(e);
	}

	void EventUser::UnregisterThis() {
		if (Module::Get())
		{
			Module::Get()->GetEventQueue()->RemoveEventUser(this);
			Callbacks.clear();
		}
	}
} // namespace sbe
