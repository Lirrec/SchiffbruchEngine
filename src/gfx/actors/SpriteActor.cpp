#include <sbe/gfx/actors/SpriteActor.hpp>

namespace sbe
{
	sf::Drawable& SpriteActor::getDrawable() {
		return sprite;
	}

	geom::rect SpriteActor::getBounds() {

		return sprite.getGlobalBounds();
	}

	SpriteActor::~SpriteActor() {

	}

	SpriteActor::SpriteActor(const sf::Sprite& sprite) : sprite(sprite) {}
} // namespace sbe
