#ifndef GENERATORS_HPP
#define GENERATORS_HPP

#include <SFML/Graphics/Color.hpp>

#include <sbe/geom/Point.hpp>
#include <sbe/gfx/particles/Particle.hpp>

#include <random>
#include <vector>
namespace sbe
{
	namespace particles
	{
		namespace generators {
			/// generates particles on a grid
			void generateGrid( std::vector<Particle>& Particles, Geom::Vec2 Size, Geom::Vec2 Count )
			{
				Particles.clear();

				std::random_device rd;
				std::default_random_engine e1(rd());
				std::uniform_int_distribution<int> uniform_dist(0, 255);

				Geom::Vec2f steps { Size.x/(float)Count.x, Size.y/(float)Count.y };

				Geom::Pointf pos;
				for ( float x = 0; x < Count.x; ++x)
				{
					pos.x += steps.x;
					pos.y = 0;
					for ( float y = 0; y < Count.y; ++y)
					{
						pos.y += steps.y;
						Particle P( pos );
						P.size = 1.0f + (uniform_dist(e1)/255.0f)/10.0f;
						P.color = sf::Color( uniform_dist(e1), uniform_dist(e1), uniform_dist(e1) );
						Particles.push_back( P );
						//Engine::out() << "Creating particle at: " << x << "," << y << std::endl;
					}
				}
			}
		}
	} // namespace particles
} // namespace sbe

#endif // GENERATORS_HPP

