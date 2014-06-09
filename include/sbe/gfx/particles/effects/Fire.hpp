#ifndef FIRE_HPP
#define FIRE_HPP

#include <SFML/Graphics.hpp>
#include <sbe/gfx/particles/Effect.hpp>
#include <sbe/geom/Geom.hpp>
#include <sbe/geom/Point.hpp>
#include <vector>

namespace sbe
{

	namespace particles
	{

		namespace effects
		{
			class Fire : public Effect
			{
			public:


				/// affect single particles
				void Affector( Particle& P, float delta) override;
				/// may insert or remove particles
				void Manipulator( std::vector<Particle>&, float ) override;

				float speed 			= 100;
				float spread 			= Geom::pi()/18;
				float maxage 			= 0.2f;
				Geom::Vec2f sizelimits 	= { 0.5, 1.5 };
				Geom::Vec2f agelimits 	= { 0, 0.1 };
				Geom::Vec2f pos 		= {0,0};
				float gametime = 0;
				int pps = 240;
				float spawntime = 0;
			};
		} // namespace effects

	} // namespace particles

} // namespace sbe

#endif // FIRE_HPP

