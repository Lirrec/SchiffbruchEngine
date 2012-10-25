#ifndef CORE_H
#define CORE_H

#include "Module.hpp"
#include "event/EventUser.hpp"

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

