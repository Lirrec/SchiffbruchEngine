#include <sbe/entity/EntityID.hpp>

#include "boost/uuid/nil_generator.hpp"

namespace sbe
{
	sbeID sbeID_namespace = boost::uuids::nil_generator()();

	namespace operators
	{
		sbeID operator
		""

		_id(const char* str, size_t) {
			boost::uuids::name_generator gen(sbeID_namespace);
			return gen(str);
		}
	}
} // namespace sbe
