#ifndef MANIPULATORS_HPP
#define MANIPULATORS_HPP


#include <sbe/Engine.hpp>
#include <sbe/geom/PointHelpers.hpp>
#include <sbe/geom/Point.hpp>
#include <sbe/gfx/particles/Particle.hpp>
#include <sbe/gfx/VertexUtils.hpp>

#include <sbe/util/FactoriesAndManipulators.hpp>

#include <sbe/geom/Point.hpp>
#include <glm/gtc/constants.hpp>

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
			inline void emitRandom(std::vector<Particle>& Particles, float, glm::vec2 Origin, float maxvelocity, glm::vec2 sizelimits)
			{
				Particle re;
				re.position = Origin;
				re.rotation = factories::randomFloat( {0, 2*glm::pi<float>() });
				float  speed = factories::randomFloat( {0, maxvelocity});
				auto tmp = gfx::rot( {0, speed}, re.rotation );
				re.velocity = {tmp.x, tmp.y};
				re.avelocity = factories::randomFloat( {-maxvelocity, maxvelocity});
				re.color = factories::randomColor();
				re.size = factories::randomFloat({sizelimits.x, sizelimits.y});

				Particles.push_back(re);
			}

			/// Emits a ray of particles
			inline void emitRay(std::vector<Particle>& Particles, float, glm::vec2 Origin, float direction, float maxspread, float velocity, glm::vec2 sizelimits, glm::vec2 agelimits)
			{
				Particle re;
				re.position = Origin;
				re.rotation = factories::randomFloat( {0, 2*glm::pi<float>() });
				float spread = factories::randomFloat( {-maxspread, maxspread} );
				float speed = factories::randomFloat( {velocity- velocity*0.1f, velocity+ velocity*0.1f} );
				//Engine::out() << "speed: " << speed << std::endl;
				auto tmp = gfx::rot( {0.0f, speed }, direction + spread );
				re.velocity = {tmp.x, tmp.y};
				re.avelocity = factories::randomFloat( {-velocity, velocity});
				re.color = factories::randomColor();
				re.size = factories::randomFloat({sizelimits.x, sizelimits.y});
				re.age = factories::randomFloat({agelimits.x, agelimits.y});

				Particles.push_back(re);
			}

			/// removes all particles with an age > 1
			inline void destroyOld(std::vector<Particle>& Particles, float)
			{
				auto pend = std::remove_if(Particles.begin(), Particles.end(), [](Particle& P){ return P.age > 1; });
				Particles.erase(pend, Particles.end());
			}
		}
	} // namespace particles
} // namespace sbe

#endif // MANIPULATORS_HPP

