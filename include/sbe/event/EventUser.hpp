#ifndef EVTLISTENER_H
#define EVTLISTENER_H

#include "sbe/event/Event.hpp"
#include "sbe/Module.hpp"
#include "EventHelper.hpp"

#include <map>

#include <sbe/util/Meta.hpp>

namespace sbe {


	/**
		Base class for all classes which receive events.
		An Event user may receive Events (and send) Events.
		A unique EventUser-id is generated for each Eventuser.

		To receive Events call RegisterForEvent() with the desired Event and a callback
		or implement the virtual HandleEvent() function.

	*/
	class EventUser {
	public:
		EventUser();

		virtual ~EventUser();

	protected:

		typedef std::function<void(Event &e)> EventHandler;

		/**
			Register for an Event by Type.
			@param EvtType the Event type
			@param priority the priority of this Eventhandler, higher means being called before all the low priority handlers
		*/
		void RegisterForEvent(const Event::EventType &EvtType, int priority = 0);

		/**
			Register for an Event by Name.
			@param EvtName the Event name
			@param priority the priority of this Eventhandler, higher means being called before all the low priority handlers
		*/
		void RegisterForEvent(const std::string &EvtName, int priority = 0);

		/**
			Register a callback for an Event by name.
			This will replace any existing callbacks.
			@param EvtName the Event name
			@param Handler a callback which is called with the event as first parameter if a event with EvtName occurs.
			@param priority the priority of this Eventhandler, higher means being called before all the low priority handlers
		*/
		void RegisterForEvent(const std::string &EvtName, const EventHandler &Handler, int priority = 0);

		/**
			Register a callback for an Event by EventType.
			This will replace any existing callbacks.
			@param EvtName the Event name
			@param Handler a callback which is called with the event as first parameter if a event with EvtName occurs.
			@param priority the priority of this Eventhandler, higher means being called before all the low priority handlers
		*/
		void RegisterForEvent(const Event::EventType &EvtType, const EventHandler &Handler, int priority = 0);


		template<HashType Hash, class Base, typename... Params>
		void RegisterMemberAsEventCallback(Base *_this, sbe::EventDef<Hash, void (Base::*)(Params...)> D,
		                                   const std::string &name, int prio = 0) {
			using ParamTuple = tuple_with_removed_refs<Params...>;

			std::cout << "register" << std::endl;

			_this->RegisterForEvent(name, [_this, D](const Event &E) {
				try {
					ParamTuple Arguments = boost::any_cast<ParamTuple>(E.cData());
					//std::cout << "Invoke" << std::endl;
					sbe::invoke_member(_this, D.member, Arguments);
				} catch (boost::bad_any_cast& e) {
					std::cout << "Event callback for member. Any cast failed: " << e.what() << std::endl;
					std::cout << "Event has type: " << E.cData().type().name() << std::endl;
					std::cout << "Member function has type: " << typeid(ParamTuple).name() << std::endl;
				}
			}, prio);
		}

		/**
			Remove this listener from all registered Events
		*/
		void UnregisterThis();


		/**
			Handle Events manually.
			This function will be called with each event that is not already handeled by a callback.
		*/
		virtual void HandleEvent(Event &) { };

	private:

		friend class EventQueue;

		void RealHandleEvent(Event &e);

		std::map<Event::EventType, EventHandler> Callbacks;
	};
} // namespace sbe
#endif // EVTLISTENER_H
