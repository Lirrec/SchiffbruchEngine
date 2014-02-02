#ifndef POSITION_HPP
#define POSITION_HPP

#include "sbe/geom/Point.hpp"
#include "sbe/entity/Component.hpp"

#include <string>
#include <vector>
#include <memory>

namespace sbe {
	namespace components {

		std::vector<ComponentInfo> PositionComponents
		{
			{ "Position2D", std::shared_ptr<CopyFactory<boost::any>>( (CopyFactory<boost::any>*)  new AssignmentConversionFactory<Geom::Point, boost::any>() ) },
			{ "TargetPosition2D", std::shared_ptr<CopyFactory<boost::any>>( (CopyFactory<boost::any>*)  new AssignmentConversionFactory<Geom::Point, boost::any>() ) }
		};
	}
}

#endif // POSITION_HPP
