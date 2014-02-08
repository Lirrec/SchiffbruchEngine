#ifndef TRANSFORMABLE_HPP
#define TRANSFORMABLE_HPP

#include "SFML/Graphics/Transformable.hpp"

inline void copyTransformable(sf::Transformable& src, sf::Transformable& dest)
{
	dest.setPosition( src.getPosition() );
	dest.setRotation( src.getRotation() );
	dest.setScale( src.getScale() );
	dest.setOrigin( src.getOrigin() );
}

#endif // TRANSFORMABLE_HPP
