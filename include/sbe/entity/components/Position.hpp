#ifndef C_POSITION_HPP
#define C_POSITION_HPP

#include "sbe/geom/Point.hpp"
#include "sbe/entity/Component.hpp"

#include <string>
#include <vector>
#include <memory>

namespace sbe {
	namespace components {
		namespace {
			std::vector<ComponentInfo> getPositionComponents()
			{
				return {
					{ "Position2D", makeComponentFactory<Geom::Point>() },
					{ "TargetPosition2D", makeComponentFactory<Geom::Point>() }
				};
			}
		}
	}
}

#endif // C_POSITION_HPP
