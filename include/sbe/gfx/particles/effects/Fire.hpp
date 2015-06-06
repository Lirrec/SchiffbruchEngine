#ifndef FIRE_HPP
#define FIRE_HPP

#include <SFML/Graphics.hpp>
#include <sbe/gfx/particles/Effect.hpp>
#include <sbe/geom/Point.hpp>
#include <sbe/geom/Rect.hpp>
#include <glm/gtc/constants.hpp>
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
				void Affector(Particle& P, float delta) override;

				/// may insert or remove particles
				void Manipulator(std::vector<Particle>&, float) override;

				float speed = 100;
				float spread = glm::pi<float>()/18;
				float maxage = 0.2f;
				glm::vec2 sizelimits = {0.5, 1.5};
				glm::vec2 agelimits = {0, 0.1};
				glm::vec2 pos = {0, 0};
				float gametime = 0;
				int pps = 240;
				float spawntime = 0;
			};
		} // namespace effects

	} // namespace particles

} // namespace sbe

#endif // FIRE_HPP

