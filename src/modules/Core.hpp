#ifndef CORE_H
#define CORE_H

#include "sbe/Module.hpp"
#include "sbe/event/EventUser.hpp"

#include <memory>

namespace sbe
{
	class EventCore;

	/// Router class for distributing threaded Events (internal)
	class Core : public Module
	{
		public:
			Core();
			virtual ~Core();
			static EventCore* 		EvtCore;
	};

} // namespace sbe
#endif // CORE_H
