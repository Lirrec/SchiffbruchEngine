#ifndef SPRITERENDERER_HPP
#define SPRITERENDERER_HPP

#include "sbe/entity/System.hpp"

#include "sbe/entity/systems/SpriteRenderer.hpp"
#include "sbe/gfx/Actor.hpp"
#include <SFML/Graphics/Sprite.hpp>

namespace sbe
{

	namespace systems
	{

		class SpriteRenderer : public SystemBuilder<SpriteRenderer>
		{
			public:
				virtual void update(const Entity& E, const sf::Time& delta) ;
				virtual void onAttach(const Entity& E);
				virtual void onDetach(const Entity& E);

			private:
				ActorID ID;
		};

		template<>
		const std::vector<std::string> SystemBuilder<SpriteRenderer>::RequirementsPlain = { "Sprite", "Position2" };
		template<>
		const std::string SystemBuilder<SpriteRenderer>::Name = "SpriteRenderer";

	} // namespace systems

} // namespace sbe

#endif // SPRITERENDERER_HPP
