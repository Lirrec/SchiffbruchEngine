#ifndef EVTHELPER_H
#define EVTHELPER_H

#include "Event.hpp"

#include <string>
#include <sbe/Module.hpp>

namespace sbe
{

	template<HashType HashValue, class F>
	class EventDef {
		static_assert(true,
		              "Member Function Pointer as template argument required.");
	};

	template<HashType HashValue, typename Base, typename... Params>
	class EventDef<HashValue, void (Base::*)(Params...)> {
	public:
		constexpr EventDef(void (Base::*fun)(Params...))
				: member(fun) { }

		//static constexpr HashType Hash = HashValue;
		void (Base::*member)(Params...);

		template<typename... Args>
		static void Queue(bool global, Args &&... args) {
			return Module::Get()->QueueEvent<tuple_with_removed_refs<Params...> >(HashValue, global, std::forward<Args>(args)...);
		}
	};

	template<HashType H, class T>
	static constexpr auto makeEventDef(T&& t) {
		return EventDef<H, T>(std::forward<T>(t));
	};

} // namespace sbe

#endif // EVTHELPER_H

