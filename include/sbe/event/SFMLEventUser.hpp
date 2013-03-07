#ifndef SFMLEVENTUSER_HPP
#define SFMLEVENTUSER_HPP

#include <SFML/Window/Event.hpp>

/**
	A abstract class for classes which can handle sfml events.
*/
class SFMLEventUser
{
	public:
	virtual ~SFMLEventUser(){}
	virtual void HandleSfmlEvent ( const sf::Event& e) = 0;
};


#endif // SFMLEVENTUSER_HPP

