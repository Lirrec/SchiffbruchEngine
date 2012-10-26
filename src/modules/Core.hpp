#ifndef CORE_H
#define CORE_H

#include "sbe/Module.hpp"
#include "sbe/event/EventUser.hpp"

#include <memory>

class EventCore;

class Core : public Module
{
	public:
		Core();
		virtual ~Core();
		static EventCore* 		EvtCore;
};

#endif // CORE_H

