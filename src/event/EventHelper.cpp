#include "event/EventHelper.h"


/*
// SFMLEventBinder

SFMLEventBinder::SFMLEventBinder( sf::Event::EventType trigger, std::function< void(sf::Event&) > action)
{
	this->action = action;
	RegisterForSFMLEvent( trigger );
}

void SFMLEventBinder::HandleEvent ( sf::Event& e )
{
	action( e);
}
*/
// EventBinder

EventBinder::EventBinder( const std::string& trigger, std::function< void(const Event&) > _action)
{
	this->action = _action;
	RegisterForEvent( trigger );
}

void EventBinder::HandleEvent ( Event& e )
{
	action( e );
}
