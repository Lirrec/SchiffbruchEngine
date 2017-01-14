#include "sbe/gfx/Actor.hpp"

#include <boost/uuid/uuid_generators.hpp>

namespace sbe
{
	Actor::Actor()
			: ID(boost::uuids::random_generator()()) {
	}

	sf::Drawable& SpriteActor::getDrawable() {
		return sprite;
	}


	geom::rect SpriteActor::getBounds() {

		return sprite.getGlobalBounds();
	}

	sf::Drawable& VertexActor::getDrawable() {
		return arr;
	}

	geom::rect VertexActor::getBounds() {
		return arr.getBounds();
	}

	sf::Drawable& DrawableActor::getDrawable() {
		return *d;
	}

	geom::rect DrawableActor::getBounds() {
		return {{0,0},{0,0}};
	}

	AnimationActor::AnimationActor(ImageSet& ImgSet)
			: Anim(ImgSet) {
	}

	void AnimationActor::update(const sf::Time& RenderTime) {
		Anim.update(RenderTime);
	}

	sf::Drawable& AnimationActor::getDrawable() {
		return Anim.getSprite();
	}

	geom::rect AnimationActor::getBounds() {
		return Anim.getSprite().getGlobalBounds();
	}

	sf::Drawable& ShapeActor::getDrawable() {
		return *Shape;
	}

	geom::rect ShapeActor::getBounds() {
		return Shape->getGlobalBounds();
	}
} // namespace sbe
