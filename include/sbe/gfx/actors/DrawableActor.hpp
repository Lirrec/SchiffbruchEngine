#ifndef SCHIFFBRUCHENGINE_DRAWABLEACTOR_HPP
#define SCHIFFBRUCHENGINE_DRAWABLEACTOR_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <memory>

#include <sbe/gfx/Actor.hpp>

namespace sbe {

	/**
		An Actor holding a sf::VertexArray
	*/
	class DrawableActor : public Actor {
	public:
		sf::Drawable& getDrawable() override;

		geom::rect getBounds() override;

		std::shared_ptr<sf::Drawable> d;
	};

} // namespace sbe
#endif //SCHIFFBRUCHENGINE_DRAWABLEACTOR_HPP
