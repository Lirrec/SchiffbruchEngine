#ifndef PARTICLEAFFECTORS_HPP
#define PARTICLEAFFECTORS_HPP

#include <sbe/geom/Point.hpp>
#include <sbe/gfx/Particle.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <vector>

namespace sbe
{
	namespace particles
	{
		namespace generators {
			/// generates particles on a grid
            void generateGrid(std::vector<Particle>& Particles, Geom::Vec2 Size, Geom::Vec2 Count );
		}

		namespace affectors {
			/// applies a gravitational pull to a particle
			void applyGravitation(Particle& P, float delta, Geom::Vec2f pos, float m);
			/// applies a gravity ( down ) to a particle
			void applyGravity(Particle& P, float delta, Geom::Vec2f direction, float factor = 1.f);
			/// moves a particle according
			void move(Particle& P,  float delta, float dampening = 0.95f);

			/// performs a collision with a bounding box
			void collideBoxed(Particle& P,  float delta, Geom::Vec2f limits);
			/// performs a collision with a bounding box ( bouncy )
			void collideBoxedBouncy(Particle& P,  float delta, Geom::Vec2f limits);
			/// performs a warp to the other side of the particle system if a particle leaves one side
			void collideWarped(Particle& P,  float delta, Geom::Vec2f limits);
			/// apply a gravitation if the left mouse button is clicked
			void applyMouseGravitation( Particle& P,  float delta );
		}

		namespace renderers {
			/// renders a particlesystem as points
			void renderPoints(std::vector<Particle>& Particles, sf::VertexArray& V);
		}
	}
}

#endif // PARTICLEAFFECTORS_HPP


