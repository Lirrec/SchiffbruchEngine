#ifndef ENTITY_ID
#define ENTITY_ID

#include <boost/uuid/uuid.hpp>

namespace sbe
{

	typedef boost::uuids::uuid sbeID;
	static sbeID sbeID_namespace;
} // namespace sbe

#endif // ENTITY_ID

