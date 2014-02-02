#include <SFML/Graphics/Sprite.hpp>

namespace sbe
{
	namespace components
	{
		char spritename[] = "Sprite";
		typedef ComponentBuilder<spritename, sf::Sprite> Sprite;
		extern template class Sprite;
	}
}
