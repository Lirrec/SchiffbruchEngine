#ifndef SPRITERENDERER_HPP
#define SPRITERENDERER_HPP

#include "sbe/entity/System.hpp"


namespace sbe
{
	class Actor;
	class Entity;

//	namespace systems
//	{
//		class SpriteRenderer;
//	} // namespace systems
//
//	extern template class SystemBuilder<systems::SpriteRenderer>;

	namespace systems
	{

		class SpriteRenderer : public SystemBuilder<SpriteRenderer>
		{
			public:
				void update(Entity& E, const sf::Time& delta) override;
				void onAttach(Entity& E) override;
				void onDetach(Entity& E) override;

			private:
				std::shared_ptr<Actor> A;
		};

	} // namespace systems

//	template<>
//	std::vector<std::string> SystemBuilder<systems::SpriteRenderer>::RequirementsPlain { "Sprite", "Position2" };
//	template<>
//	std::string SystemBuilder<systems::SpriteRenderer>::Name = "SpriteRenderer";

//	template<>
//	std::vector<std::string> SystemBuilder<systems::SpriteRenderer>::RequirementsPlain;
//	extern template std::vector<std::string> SystemBuilder<systems::SpriteRenderer>::RequirementsPlain;

	//extern template const std::string SystemBuilder<systems::SpriteRenderer>::Name;
	//template<>
	//const std::string SystemBuilder<systems::SpriteRenderer>::Name;

} // namespace sbe

#endif // SPRITERENDERER_HPP
