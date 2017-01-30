#include <sbe/entity/systems/SpriteRenderer.hpp>

#include <boost/uuid/uuid_io.hpp>



#include <sbe/gfx/Renderer.hpp>
#include <sbe/util/Transformable.hpp>

#include <sbe/entity/Entity.hpp>

#include <sbe/gfx/actors/SpriteActor.hpp>

#include <sbe/event/Event.hpp>
#include <sbe/Module.hpp>


namespace sbe
{
	template<>
	std::vector<std::string> SystemBuilder<systems::SpriteRenderer>::RequirementsPlain{"Sprite", "Position2D", "RenderLayer"};
	template<>
	std::string SystemBuilder<systems::SpriteRenderer>::Name = "SpriteRenderer";


	namespace systems
	{
		using namespace sbe::operators;

		void SpriteRenderer::onEntityUpdate(Entity& E) {
			A->sprite = E.C<sf::Sprite>("Sprite"_cId);
			copyTransformable(E.C<sf::Transformable&>("Transformable"_cId), A->sprite);
		}

		void SpriteRenderer::onAttach(Entity& E) {
			A = std::make_shared<SpriteActor>( E.C<sf::Sprite>("Sprite"_cId) );
			copyTransformable(E.C<sf::Transformable&>("Transformable"_cId), A->sprite);

			sbe::Renderer::addActorEvent().Queue(true, std::dynamic_pointer_cast<Actor>(A), E.C<unsigned int>("RenderLayer"_cId));
		}

		void SpriteRenderer::onDetach(Entity& E) {
			Engine::out() << "SpriteRenderer onDetach " << E.getID() << std::endl;
			Module::Get()->QueueEvent("REMOVE_ACTOR", true, A);
			A.reset();
		}


	} // namespace systems

} // namespace sbe
