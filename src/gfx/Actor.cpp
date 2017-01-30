#include <sbe/gfx/Actor.hpp>

#include <boost/uuid/uuid_generators.hpp>

namespace sbe
{
	Actor::Actor()
			: ID(boost::uuids::random_generator()()) {
	}






} // namespace sbe
