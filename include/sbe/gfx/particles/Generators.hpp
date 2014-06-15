#ifndef GENERATORS_HPP
#define GENERATORS_HPP

#include <SFML/Graphics/Color.hpp>

#include <sbe/geom/Point.hpp>
#include <sbe/gfx/particles/Particle.hpp>

#include <sbe/util/FactoriesAndManipulators.hpp>

#include <random>
#include <vector>
namespace sbe
{
	namespace particles
	{
		namespace generators {
			/// generates particles on a grid
			void generateGrid( std::vector<Particle>& Particles, glm::ivec2 Size, glm::ivec2 Count, glm::vec2 particlesizelimits )
			{
				Particles.clear();

				glm::vec2 steps { Size.x/(float)Count.x, Size.y/(float)Count.y };

				glm::point2 pos;
				for ( float x = 0; x < Count.x; ++x)
				{
					pos.x += steps.x;
					pos.y = 0;
					for ( float y = 0; y < Count.y; ++y)
					{
						pos.y += steps.y;
						Particle P( pos );
						P.size = factories::randomFloat(particlesizelimits);
						P.color = sf::Color::White;
						Particles.push_back( P );
						//Engine::out() << "Creating particle at: " << x << "," << y << std::endl;
					}
				}
			}
		}
	} // namespace particles
} // namespace sbe

#endif // GENERATORS_HPP

