#include "sbe/Engine.hpp"
#include "sbe/entity/System.hpp"
#include "sbe/entity/EntityManager.hpp"

namespace sbe {
    namespace operators {
        sbeID operator "" _sId(const char *str, size_t) {
            return Engine::GetEntityMgr()->lookupSystemID(str);
        }
    }
	System::System() {}
	System::~System() {}
}

