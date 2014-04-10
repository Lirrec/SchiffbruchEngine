#ifndef MEMBERS_HPP
#define MEMBERS_HPP

#include <sbe/geom/Point.hpp>
#include <sbe/gfx/particles/Particle.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sbe
{
	namespace particles
	{
		/// returns a random float in range limits
		float randomFloat(Geom::Vec2f limits);
		/// returns a random point inside limits
		Geom::Vec2f randomPoint(Geom::Rectf limits);
		/// returns a random color
		Geom::Vec2f randomColor();

		namespace members {
			inline Geom::Vec2f& velocity( Particle &P ) { return P.velocity; }
			inline Geom::Vec2f& position( Particle &P ) { return P.position; }
			inline float& rotation( Particle &P ) { return P.rotation; }
			inline float& avelocity( Particle &P ) { return P.avelocity; }
			inline float& age( Particle &P ) { return P.age; }
			inline float& size( Particle &P ) { return P.size; }
			inline sf::Color& color( Particle &P ) { return P.color; }
		}
	}
}

#endif // MEMBERS_HPP
