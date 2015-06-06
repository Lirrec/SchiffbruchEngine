#include "sbe/entity/systems/SpriteRenderer.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include "sbe/util/Transformable.hpp"

#include "sbe/entity/Entity.hpp"
#include "sbe/gfx/Actor.hpp"
#include "sbe/event/Event.hpp"
#include "sbe/Module.hpp"
#include "sbe/geom/Point.hpp"

#include <vector>
#include <string>

#include <boost/uuid/uuid_io.hpp>

#include "sbe/Engine.hpp"
#include <iostream>

namespace sbe
{
	template<>
	std::vector<std::string> SystemBuilder<systems::SpriteRenderer>::RequirementsPlain { "Sprite", "Position2D", "RenderLayer" };
	template<>
	std::string SystemBuilder<systems::SpriteRenderer>::Name = "SpriteRenderer";


	namespace systems
	{
        using namespace sbe::operators;
		void SpriteRenderer::onEntityUpdate(Entity& E)
		{
			A->sprite = E.C<sf::Sprite>("Sprite"_cId);
			copyTransformable( E.C<sf::Transformable&>("Transformable"_cId), A->sprite);
		}

		void SpriteRenderer::onAttach(Entity& E)
		{
			A.reset( new SpriteActor );
			A->sprite = E.C<sf::Sprite>("Sprite"_cId);
			copyTransformable( E.C<sf::Transformable&>("Transformable"_cId), A->sprite);
			Module::Get()->QueueEvent( Event("ADD_ACTOR", std::make_pair(std::dynamic_pointer_cast<Actor>(A), E.C<int>("RenderLayer"_cId))), true 	);
		}

		void SpriteRenderer::onDetach(Entity& E)
		{
			Engine::out() << "SpriteRenderer onDetach " << E.getID() << std::endl;
			Module::Get()->QueueEvent( Event("REMOVE_ACTOR", A), true );
			A.reset();
		}


	} // namespace systems

} // namespace sbe
