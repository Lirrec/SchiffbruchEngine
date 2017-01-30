#ifndef SPRITERENDERER_HPP
#define SPRITERENDERER_HPP

#include <sbe/entity/System.hpp>


namespace sbe
{
	class SpriteActor;

	class Entity;

	namespace systems
	{

		class SpriteRenderer : public SystemBuilder<SpriteRenderer>
		{
		public:
			void onEntityUpdate(Entity& E) override;

			void onAttach(Entity& E) override;

			void onDetach(Entity& E) override;

		private:
			std::shared_ptr<SpriteActor> A;
		};

	} // namespace systems

} // namespace sbe

#endif // SPRITERENDERER_HPP
