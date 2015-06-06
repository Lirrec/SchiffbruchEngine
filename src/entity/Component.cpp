#include "sbe/entity/Component.hpp"

#include "sbe/entity/EntityManager.hpp"

namespace sbe
{
    namespace operators {
        sbeID operator "" _cId(const char *str, size_t) {
            return Engine::GetEntityMgr()->lookupComponentID(str);
        }
    }
}
