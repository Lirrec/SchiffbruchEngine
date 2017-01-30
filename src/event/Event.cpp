#include <sbe/event/Event.hpp>

#include <boost/lexical_cast.hpp>
#include <sbe/util/SimpleModule.hpp>

#include <sbe/event/EventHelper.hpp>

// local includes
#include "EventCore.hpp"

namespace sbe
{
	Event::Event(const std::string& EventName) {
		init(EventName, 0);
	}

	Event::Event(const std::string& EventName, const boost::any& _Data) {
		init(EventName, _Data);
	}

	Event::Event(const EventType &Type) {
		this->Type = Type;
	}

	Event::Event(const EventType &Type, const boost::any &_Data) {
		this->Type = Type;
		EvtData = _Data;
	}

	Event::EventType Event::hashName(const std::string& EventName) {
		return EventCore::getInstance()->GetEventHash(EventName);
	}

	void Event::init(const std::string& EventName, const boost::any& _Data) {
		if (!EventCore::getInstance()->HasEvent(EventName))
			EventCore::getInstance()->RegisterEventName(EventName);
		EvtData = _Data;
		Type = hashName(EventName);
	}

	std::string Event::getDebugName() {
		std::string re("");
		re = EventCore::getInstance()->GetEventName(Type);
		re += " - ";
		re += boost::lexical_cast<std::string>(Type);
		return re;
	}

	bool Event::Is(const std::string& Name) const {
		return Type == EventCore::getInstance()->GetEventHash(Name);
	}

	bool Event::Is(const std::string& Name, const std::type_info& type) const {
		if (!Is(Name)) return false;
		if (EvtData.type() != type)
		{
			Engine::out(Engine::ERROR) << "[Event] " << Name << " has wrong Datatype (has: " << EvtData.type().name() << ", wants: " << type.name() << ") !" << std::endl;
			return false;
		}

		return true;
	}
} // namespace sbe
