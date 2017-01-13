#include <SFML/Graphics/Sprite.hpp>

#include "sbe/entity/Component.hpp"

namespace sbe
{
	namespace components
	{
		namespace
		{
			std::vector<ComponentInfo> getSFMLComponents() {
				return {
						{"Sprite",      makeComponentFactory<sf::Sprite>()},
						{"RenderLayer", makeComponentFactory<unsigned int>()}
				};
			}
		}
	}
}
