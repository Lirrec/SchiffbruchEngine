#include "Core.hpp"

#include "../event/EventCore.hpp"

namespace sbe
{

	EventCore* Core::EvtCore;

	Core::Core() {
		EvtCore = new EventCore();
	}

	Core::~Core() {
		delete EvtCore;
		EvtCore = nullptr;
	}

} // namespace sbe
