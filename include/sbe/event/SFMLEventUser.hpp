#ifndef SFMLEVENTUSER_HPP
#define SFMLEVENTUSER_HPP

#include <SFML/Window/Event.hpp>

/**
	A base class for classes which can handle sfml events.
*/
class SFMLEventUser
{
	public:
	virtual ~SFMLEventUser(){}

	/**
		Handle an sfml event.
		@param e const reference to the current event
	*/
	virtual void HandleSfmlEvent ( const sf::Event& e) = 0;
};


#endif // SFMLEVENTUSER_HPP

