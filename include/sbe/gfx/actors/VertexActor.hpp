#ifndef SCHIFFBRUCHENGINE_VERTEXACTOR_HPP
#define SCHIFFBRUCHENGINE_VERTEXACTOR_HPP

#include <sbe/gfx/Actor.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace sbe
{

	/**
		An Actor holding a sf::VertexArray
	*/
	class VertexActor : public Actor
	{
	public:
		sf::Drawable& getDrawable()  override;
		geom::rect getBounds() override;

		sf::VertexArray arr;
	};

} // namespace sbe


#endif //SCHIFFBRUCHENGINE_VERTEXACTOR_HPP
