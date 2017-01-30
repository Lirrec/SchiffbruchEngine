#ifndef SCHIFFBRUCHENGINE_SPRITEACTOR_HPP
#define SCHIFFBRUCHENGINE_SPRITEACTOR_HPP

#include <sbe/gfx/Actor.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sbe
{
	/**
		An Actor holding a simple sf::Sprite
	*/
	class SpriteActor : public Actor
	{
	public:
		SpriteActor(const sf::Sprite& sprite);

		virtual ~SpriteActor();

		sf::Drawable& getDrawable()  override;
		geom::rect getBounds() override;

		sf::Sprite sprite;

	};

} // namespace sbe


#endif //SCHIFFBRUCHENGINE_SPRITEACTOR_HPP
