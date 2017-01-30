#include <sbe/gfx/actors/DrawableActor.hpp>


namespace sbe
{

	sf::Drawable& DrawableActor::getDrawable() {
		return *d;
	}

	geom::rect DrawableActor::getBounds() {
		return {{0,0},{0,0}};
	}

} // namespace sbe