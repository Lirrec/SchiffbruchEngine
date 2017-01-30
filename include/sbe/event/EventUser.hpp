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



		template <class Arg>
		void RegisterForEvent(const std::string &EvtName, std::function<void(Arg)> Handler, int priority = 0) {
			using ArgData = std::remove_reference_t<Arg>;
			auto wrapHandler = [=](Event& e){
				if (e.Is(EvtName, typeid(ArgData))) {
					Handler(boost::any_cast<ArgData>(e.Data()));
				}
			};

			RegisterForEvent( EvtName, wrapHandler, priority);
		}

		template <class Arg, class Arg2>
		void RegisterForEvent(const std::string &EvtName, std::function<void(Arg, Arg2)> Handler, int priority = 0) {

			using ParamTuple = std::tuple<std::remove_reference_t<Arg>,std::remove_reference_t<Arg2>>;
			auto wrapHandler = [=](Event& e) {
				if (e.Is(EvtName, typeid(ParamTuple))) {
					ParamTuple t = boost::any_cast<ParamTuple>(e.Data());
					sbe::invoke(Handler, t);
				}
			};

			RegisterForEvent(EvtName, wrapHandler, priority);
		}

		template <class Arg, class Arg2, class Arg3>
		void RegisterForEvent(const std::string &EvtName, std::function<void(Arg, Arg2, Arg3)> Handler, int priority = 0) {

			using ParamTuple = std::tuple<std::remove_reference_t<Arg>,std::remove_reference_t<Arg2>,std::remove_reference_t<Arg3>>;
			auto wrapHandler = [=](Event& e) {
				if (e.Is(EvtName, typeid(ParamTuple))) {
					ParamTuple t = boost::any_cast<ParamTuple>(e.Data());
					sbe::invoke(Handler, t);
				}
			};

			RegisterForEvent(EvtName, wrapHandler, priority);
		}

		template <class Arg, class Arg2, class Arg3, class Arg4>
		void RegisterForEvent(const std::string &EvtName, std::function<void(Arg, Arg2, Arg3, Arg4)> Handler, int priority = 0) {

			using ParamTuple = std::tuple<std::remove_reference_t<Arg>,std::remove_reference_t<Arg2>,std::remove_reference_t<Arg3>,std::remove_reference_t<Arg4>>;
			auto wrapHandler = [=](Event& e) {
				if (e.Is(EvtName, typeid(ParamTuple))) {
					ParamTuple t = boost::any_cast<ParamTuple>(e.Data());
					sbe::invoke(Handler, t);
				}
			};

			RegisterForEvent(EvtName, wrapHandler, priority);
		}

		/*template <class... Args>
		void RegisterForEvent(const std::string &EvtName, std::function<void(Args...)> Handler, int priority = 0) {

			using ParamTuple = std::tuple<std::remove_reference_t<Args>...>;
			auto& wrapHandler = [=](Event& e) {
				if (e.Is(EvtName, typeid(ParamTuple))) {
					ParamTuple t = boost::any_cast<ParamTuple>(e.Data());
					sbe::invoke(Handler, t);
				}
			};

			RegisterForEvent(EvtName, wrapHandler, priority);
		}*/

		/**
		 * Register a member function for an Event. When the Event occurs the member function will be called with the appropriate Parameters.
		 * This requires the Event to contain a tuple consisting of exactly the parameters of the function ( although the tuple may not contain references ).
		 *
		 * @see sbe::makeEventDef and sbe::Renderer::addActorEvent() for an example of how to provide an Interface to allow creation of an appropriate Event.
		 *
		 */
		template<HashType Hash, class Base, typename... Params>
		void RegisterMemberAsEventCallback(Base *_this, sbe::EventDef<Hash, void (Base::*)(Params...)> D, const std::string& EventName, int prio = 0) {
			using ParamTuple = tuple_with_removed_refs<Params...>;

			// side-effect: this registers the EventName and its hash as a valid Event so later usage of the EventDef ( which only retains the hash )
			// will not throw errors because the string corresponding to the hash is not known
			Event e(EventName);

			_this->RegisterForEvent(Hash, [_this, D](const Event &E) {
				try {
					ParamTuple Arguments = boost::any_cast<ParamTuple>(E.cData());
					//std::cout << "Invoke" << std::endl;
					sbe::invoke_member(_this, D.member, Arguments);
				} catch (boost::bad_any_cast& e) {
					Engine::out(Engine::ERROR) << "Event callback for member. Any cast failed: " << e.what() << std::endl;
					Engine::out(Engine::ERROR) << "Event has type: " << E.cData().type().name() << std::endl;
					Engine::out(Engine::ERROR) << "Member function has type: " << typeid(ParamTuple).name() << std::endl;
				}
			}, prio);
		}

		/**
		 * Calls a member function with no parameters if the given Event occurs
		 * @tparam Base
		 */
		template<class Base>
		void RegisterMemberAsEventCallback(Base *_this, void(Base::*member)(), const std::string& EventName, int prio = 0) {
			_this->RegisterForEvent(EventName, [_this, member](const Event &E) {
				(_this->*member)();
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
