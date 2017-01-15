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

	/**
	 * This class provides a single member function Queue() which creates an Event in the correct way to work with
	 * sbe::EventUser::RegisterMemberAsEventCallback().
	 * @tparam HashValue
	 * @tparam Base
	 * @tparam Params
	 */
	template<HashType HashValue, typename Base, typename... Params>
	class EventDef<HashValue, void (Base::*)(Params...)> {
	public:
		constexpr EventDef(void (Base::*fun)(Params...))
				: member(fun) { }

		//static constexpr HashType Hash = HashValue;
		void (Base::*member)(Params...);

		template<typename... Args>
		static void Queue(bool global, Args &&... args) {
			Module::Get()->QueueEvent(HashValue, global, std::forward<Args>(args)...);
		}
	};

	/**
	 * Create an EventDef for a given member function pointer
	 * @tparam H
	 * @tparam T
	 * @param t the member function pointer which the EventDef should use
	 * @return
	 */
	template<HashType H, class T>
	static constexpr auto makeEventDef(T&& t) {
		return EventDef<H, T>(std::forward<T>(t));
	};

} // namespace sbe

#endif // EVTHELPER_H

