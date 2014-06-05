
#include <sbe/gfx/particles/effects/Fire.hpp>
#include <sbe/gfx/particles/Manipulators.hpp>
#include <sbe/gfx/particles/Affectors.hpp>

#include <sbe/util/FactoriesAndManipulators.hpp>

#include <sbe/Engine.hpp>

namespace sbe
{
	namespace particles
	{
		namespace effects
		{
			sf::Color firecolor(float age)
			{
				sf::Color orange(0xff, 0xa5, 0x0);

				if ( age < 0.2)
					return sbe::interpolate::interpolateColorRGB( sf::Color(0,0,255,0),sf::Color::Blue, age/0.2);
				else if ( age < 0.5)
					return sbe::interpolate::interpolateColor( sf::Color(255,255,0,100), orange, (age-0.2)/0.5);
				else
					return sbe::interpolate::interpolateColorRGB( orange, sf::Color(0xff, 0xa5, 0x0,255), (age-0.7)/0.3);
			}

			void Fire::Affector( Particle& P, float delta)
			{
				using namespace sbe::particles;
				using namespace std::placeholders;

				float tmp = P.age * Geom::pi();
				P.velocity.x = 0;
				//float disp = std::sin(tmp*2)*(1/P.age) + std::sin(tmp*5.0f);
				float disp = std::sin(tmp)*5 + P.avelocity * std::sin(tmp) + std::sin(gametime+(tmp*2))*2;
				P.position.x = pos.x + disp;

//				Engine::out() << delta << std::endl;

				affectors::move( P, delta, 0.8f);
				affectors::age( P, delta, maxage);
				P.color = firecolor(P.age);
			}

			void Fire::Manipulator( std::vector<Particle>& Ps, float delta )
			{
				using namespace sbe::particles;
				gametime += delta;
				spawntime += delta;
				while ( spawntime > (1.0f/pps))
				{
					spawntime  -= (1.0f/pps);
					manipulators::emitRay( Ps,delta, pos, Geom::pi(), spread, speed, sizelimits, agelimits );
					float tmp = speed * maxage / 2.0f;
					if ( Ps.size() ) Ps.back().avelocity = factories::randomFloat( {-tmp, tmp} );
				}


			}

		} // namespace effects


	} // namespace particles

} // namespace sbe



