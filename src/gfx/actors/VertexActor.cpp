#include <sbe/gfx/actors/VertexActor.hpp>


namespace sbe
{


	sf::Drawable& VertexActor::getDrawable() {
		return arr;
	}

	geom::rect VertexActor::getBounds() {
		return arr.getBounds();
	}

} // namespace sbe