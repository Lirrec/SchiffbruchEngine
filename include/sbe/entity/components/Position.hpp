#ifndef C_POSITION_HPP
#define C_POSITION_HPP

#include "sbe/geom/Point.hpp"
#include "sbe/entity/Component.hpp"

#include <SFML/Graphics/Transformable.hpp>

#include <vector>
#include <memory>

namespace sbe {
	namespace components {
		namespace {
			std::vector<ComponentInfo> getPositionComponents()
			{
				return {
					{ "Transformable", makeComponentFactory<sf::Transformable>() },
					{ "TargetPosition2D", makeComponentFactory<Geom::Point>() }
				};
			}
		}
	}
}

#endif // C_POSITION_HPP
