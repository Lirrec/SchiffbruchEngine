#include "sbe/entity/EntityID.hpp"

#include "boost/uuid/nil_generator.hpp"

namespace sbe
{
	static sbeID sbeID_namespace = boost::uuids::nil_generator()();
} // namespace sbe
