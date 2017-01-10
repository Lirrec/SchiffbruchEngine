#include "Core.hpp"

#include "../event/EventCore.hpp"

namespace sbe
{

	std::unique_ptr<EventCore> Core::EvtCore;

	Core::Core() {
		EvtCore.reset( new EventCore );
	}

	Core::~Core() {

	}

} // namespace sbe
