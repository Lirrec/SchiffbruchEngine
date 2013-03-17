#ifndef EVTHELPER_H
#define EVTHELPER_H

#include "Event.hpp"
#include "EventUser.hpp"

#include <string>

namespace sbe
{


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

	/**
		Binds the occurence of a given Event to a std::function ( lambda, function pointer, member pointer, functor, etc )
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
} // namespace sbe

#endif // EVTHELPER_H

