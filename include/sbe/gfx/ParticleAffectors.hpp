#ifndef PARTICLEAFFECTORS_HPP
#define PARTICLEAFFECTORS_HPP

#include <sbe/geom/Point.hpp>
#include <sbe/gfx/Particle.hpp>
#include <sbe/gfx/ParticleSystem.hpp>

#include <SFML/Graphics/Color.hpp>

#include <vector>

namespace sf {class VertexArray;}

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

		namespace generators {
			/// generates particles on a grid
            void generateGrid(std::vector<Particle>& Particles, Geom::Vec2 Size, Geom::Vec2 Count );
		}

		namespace manipulators {
			/// Emits particles with random velocities
            void emitRandom(std::vector<Particle>& Particles, float, Geom::Vec2f Origin, float maxvelocity );
			/// Emits a ray of particles
			void emitRay(std::vector<Particle>& Particles, float, Geom::Vec2f Origin, float direction, float maxspread, float velocity);
			/// removes all particles with an age > 1
			void destroyOld(std::vector<Particle>& Particles, float);
		}

		namespace affectors {

			/// applies a gravitational pull to a particle
			void applyGravitation(Particle& P, float delta, Geom::Vec2f pos, float m);
			/// applies a gravity ( down ) to a particle
			void applyGravity(Particle& P, float delta, Geom::Vec2f direction, float factor = 1.f);

			/// moves a particle according
			void move(Particle& P,  float delta, float dampening = 0.95f);
			/// increases the age of a particle
			void age(Particle& P, float delta, float maxage);

			/// performs a collision with a bounding box
			void collideBoxed(Particle& P,  float delta, Geom::Vec2f limits);
			/// performs a collision with a bounding box ( bouncy )
			void collideBoxedBouncy(Particle& P,  float delta, Geom::Vec2f limits, float forceloss, float min);
			/// performs a warp to the other side of the particle system if a particle leaves one side
			void collideWarped(Particle& P,  float delta, Geom::Vec2f limits);



			template<typename T>
			void set( Particle& P, float delta, std::function<T&(Particle&)> member, std::function<T(float)> value )
			{
				member() = value(delta);
			}

		}

        namespace globalaffectors {
			/**
				apply a affector if the left mouse button is clicked
			*/
			void applyMouseAffector( Particle::Iterator start, Particle::Iterator end,  float delta, std::function<void(Particle&,float,Geom::Vec2f)> A);
        }

		namespace renderers {
			/// renders a particlesystem as points
			void renderPoints(std::vector<Particle>& Particles, sf::VertexArray& V);
			/// renders a particlesystem as points
			void renderQuads(std::vector<Particle>& Particles, sf::VertexArray& V, float size= 1.0f);
		}
	}
}

#endif // PARTICLEAFFECTORS_HPP


