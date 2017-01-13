//
// Created by gfwl on 13.01.2017.
//

#ifndef SCHIFFBRUCHENGINE_EVENTBINDER_HPP
#define SCHIFFBRUCHENGINE_EVENTBINDER_HPP

#include <functional>
#include <string>

#include <sbe/Event/EventUser.hpp>

namespace sbe {

	/*
		class SFMLEventBinder : public SFMLEventUser
		{
			public:
				SFMLEventBinder( sf::Event::EventType trigger, std::function< void(sf::Event&) > action);
				void HandleEvent ( sf::Event& e ) override;
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
			EventBinder(const std::string& trigger, std::function<void(const Event&)> _action);

			virtual ~EventBinder() { };

		private:
			std::function<void(const Event&)> action;
	};
}

#endif //SCHIFFBRUCHENGINE_EVENTBINDER_HPP
