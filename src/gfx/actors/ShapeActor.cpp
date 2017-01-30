#include <sbe/gfx/actors/ShapeActor.hpp>

namespace sbe
{

	sf::Drawable& ShapeActor::getDrawable() {
		return *Shape;
	}

	geom::rect ShapeActor::getBounds() {
		return Shape->getGlobalBounds();
	}

} // namespace sbe