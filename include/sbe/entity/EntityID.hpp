#ifndef ENTITY_ID
#define ENTITY_ID

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/name_generator.hpp>

namespace sbe
{

	typedef boost::uuids::uuid sbeID;
	static sbeID sbeID_namespace;

    namespace operators {
        sbeID operator "" _id(const char* str, size_t);
    }
} // namespace sbe

#endif // ENTITY_ID

