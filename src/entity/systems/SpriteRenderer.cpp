#include "sbe/entity/systems/SpriteRenderer.hpp"

#include <SFML/Graphics/Sprite.hpp>

#include "sbe/entity/Entity.hpp"
#include "sbe/gfx/Actor.hpp"
#include "sbe/event/Event.hpp"
#include "sbe/Module.hpp"

#include <vector>
#include <string>



namespace sbe
{
	template<>
	std::vector<std::string> SystemBuilder<systems::SpriteRenderer>::RequirementsPlain { "Sprite", "Position2" };
	template<>
	std::string SystemBuilder<systems::SpriteRenderer>::Name = "SpriteRenderer";


	namespace systems
	{



		void SpriteRenderer::update(Entity& E, const sf::Time& delta)
		{

		}

		void SpriteRenderer::onAttach(Entity& E)
		{
			SpriteActor* sa = new SpriteActor();
			sa->sprite = E.C<sf::Sprite>("Sprite");

			A.reset( (Actor*) sa );
			Module::Get()->QueueEvent( Event("ADD_ACTOR", A), true );
		}

		void SpriteRenderer::onDetach(Entity& E)
		{
			Module::Get()->QueueEvent( Event("REMOVE_ACTOR", A), true );
			A.reset();
		}


	} // namespace systems

} // namespace sbe
