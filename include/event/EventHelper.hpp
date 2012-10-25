#ifndef EVTHELPER_H
#define EVTHELPER_H

#include "Event.hpp"
#include "EventUser.hpp"

#include <string>

/*

class SFMLEventBinder : SFMLEventUser
{
	public:
		SFMLEventBinder( sf::Event::EventType trigger, std::function< void(sf::Event&) > action);
		virtual void HandleEvent ( sf::Event& e );
		virtual ~SFMLEventBinder() {};

	private:
		std::function< void(sf::Event&) > action;
};


*/
class EventBinder : EventUser
{
	public:
		EventBinder( const std::string& trigger, std::function< void(const Event&) > _action);
		virtual void HandleEvent ( Event& e );
		virtual ~EventBinder() {};

	private:
		std::function< void(const Event&) > action;
};

#endif // EVTHELPER_H

