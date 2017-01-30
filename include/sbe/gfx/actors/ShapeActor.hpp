#ifndef SCHIFFBRUCHENGINE_SHAPEACTOR_HPP
#define SCHIFFBRUCHENGINE_SHAPEACTOR_HPP

#include <sbe/gfx/Actor.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <memory>

namespace sbe
{


	/**
		An Actor holding a simple sf::Shape
	*/
	class ShapeActor : public Actor
	{
	public:
		sf::Drawable& getDrawable()  override;
		geom::rect getBounds() override;

		std::shared_ptr<sf::Shape> Shape;
	};

} // namespace sbe


#endif //SCHIFFBRUCHENGINE_SHAPEACTOR_HPP
