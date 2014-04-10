#ifndef MANIPULATORS_HPP
#define MANIPULATORS_HPP


#include <sbe/geom/PointHelpers.hpp>
#include <sbe/geom/Point.hpp>
#include <sbe/gfx/particles/Particle.hpp>
#include <sbe/gfx/VertexUtils.hpp>

#include <sbe/util/FactoriesAndManipulators.hpp>

#include <random>
#include <vector>

#include <algorithm>

namespace sbe
{
	namespace particles
	{
		namespace manipulators
		{
			/// Emits particles with random velocities
			void emitRandom(std::vector<Particle>& Particles, float, Geom::Vec2f Origin, float maxvelocity, Geom::Vec2f sizelimits)
			{
				Particle re;
				re.position = Origin;
				re.rotation = factories::randomFloat( {0, 2*Geom::pi() });
				float  speed = factories::randomFloat( {0, maxvelocity});
				auto tmp = gfx::rot( {0, speed}, re.rotation );
				re.velocity = {tmp.x, tmp.y};
				re.avelocity = factories::randomFloat( {-maxvelocity, maxvelocity});
				re.color = factories::randomColor();
				re.size = factories::randomFloat({sizelimits.x, sizelimits.y});

				Particles.push_back(re);
			}

			/// Emits a ray of particles
			void emitRay(std::vector<Particle>& Particles, float, Geom::Vec2f Origin, float direction, float maxspread, float velocity, Geom::Vec2f sizelimits, Geom::Vec2f agelimits)
			{
				Particle re;
				re.position = Origin;
				re.rotation = factories::randomFloat( {0, 2*Geom::pi() });
				float spread = factories::randomFloat( {-maxspread, maxspread} );
				auto tmp = gfx::rot( {0, velocity}, direction + spread );
				re.velocity = {tmp.x, tmp.y};
				re.avelocity = factories::randomFloat( {-velocity, velocity});
				re.color = factories::randomColor();
				re.size = factories::randomFloat({sizelimits.x, sizelimits.y});
				re.age = factories::randomFloat({agelimits.x, agelimits.y});

				Particles.push_back(re);
			}

			/// removes all particles with an age > 1
			void destroyOld(std::vector<Particle>& Particles, float)
			{
				std::remove_if(Particles.begin(), Particles.end(), [](Particle& P){ return P.age>1; });
			}
		}
	} // namespace particles
} // namespace sbe

#endif // MANIPULATORS_HPP

