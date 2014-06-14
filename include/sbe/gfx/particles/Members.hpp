#ifndef MEMBERS_HPP
#define MEMBERS_HPP

#include <sbe/geom/Point.hpp>
#include <sbe/gfx/particles/Particle.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sbe
{
	namespace particles
	{
		/*template<class T>
		class IntervallCaller
		{
			IntervallCaller(float callspersecond = 1, T& f, V)
			 : F(f)
			{
				interval = 1/callspersecond;
			}

			void operator()(std::vector<Particle>& V, float delta)
			{
				accu += delta;
				if ( accu > interval )
				{
					accu -= interval;
					F(V, delta)
				}
			}

			void operator()(Particle& P, float delta)
			{
				accu += delta;
				if ( accu > interval )
				{
					accu -= interval;
					F(P, delta)
				}
			}

			void operator()(float delta)
			{
				accu += delta;
				if ( accu > interval )
				{
					accu -= interval;
					F(delta)
				}
			}

			T& F;
			float interval = 1;
			float accu = 0;
		};*/


		namespace members {
			inline glm::ivec2f& velocity( Particle &P ) { return P.velocity; }
			inline glm::ivec2f& position( Particle &P ) { return P.position; }
			inline float& rotation( Particle &P ) { return P.rotation; }
			inline float& avelocity( Particle &P ) { return P.avelocity; }
			inline float& age( Particle &P ) { return P.age; }
			inline float& size( Particle &P ) { return P.size; }
			inline sf::Color& color( Particle &P ) { return P.color; }
		}
	}
}

#endif // MEMBERS_HPP
