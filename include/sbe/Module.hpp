#ifndef MODULE_H
#define MODULE_H

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <memory>
#include <string>
#include <list>
#include <sbe/util/Hash.hpp>
#include <sbe/util/Meta.hpp>
#include <iostream>

namespace boost {
	class thread;

	class barrier;
}

namespace sbe {
	class TickControl;

	class EventQueue;

	class Event;

	class GameBase;

	/**
		This class contains configuration parameters for the behaviour and
		default settings of a module.
		It provides a clear way to set all required parameters to run a module.
		@see GameBase::RegisterModule()
	*/
	struct ModuleStartInfo {
		/**
			Default Constructor.
			The configuration will be as following:
			Name: "_NameNotSet"
			TicksPerSecond: 100
			TickEvent: none
			useEventQueue: false
		*/
		ModuleStartInfo()
				: Name("_NameNotSet"), desiredTicksPerSecond(100), useEventQueue(true) {
			TickEvt = std::shared_ptr<Event>();
		}

		/** Verbose Constructor.
			provides all parameters in the constructor with default-values
		*/
		ModuleStartInfo(std::string name, int tps = 100,
		                std::shared_ptr<Event> Evt = std::shared_ptr<Event>(),
		                bool useQueue = true)
				: Name(name), desiredTicksPerSecond(tps), TickEvt(Evt), useEventQueue(useQueue) {
		}

		/// The name of the module to create
		std::string Name;
		/** @see Module::SetTPS()  */
		int desiredTicksPerSecond;
		/// @see Module::SetTickEvent()
		std::shared_ptr<Event> TickEvt;

		/** Does the Modules require an event queue.
			All user created modules need to use an eventqueue to be able to process events.
		*/
		bool useEventQueue;
	};

	/**
		A Game Module which runs in its own Thread.
		This is the basic building block of which user applications should consist.
		Modules provide an easy way to separate application code into threads without any extra code required by the user.
		Events will be sent and synchronized between threads if requested.

		@warning 	Any code using references to Module::Get() ( e.g. in the EventUser class on destruct ) will segfault when called
		            in the modules destructor.
		            MAKE SURE YOU DESTROY ANYTHING INSIDE A MODULE
					USING EVENTUSER OR Module::Get() IN THE DeInit() METHOD!

		Modules have their own local eventqueue and register on start with the global EventCore.
	*/
	class Module : sf::NonCopyable {
	public:
		Module();

		virtual ~Module();

		/// Static module getter. Use this in your code to access the current module.
		static Module *Get();

		/// Static EventQueue getter. Returns the Eventqueue of the current Module
		static EventQueue *Evt();

		///	Starts the module thread
		virtual void StartModule(const ModuleStartInfo &m);

		/// returns the modules name
		const std::string &GetName() const;

		// TODO: implement routing
		// virtual std::forward_list<std::string> ReturnDesiredEvents() = 0;

		/// return evtq non-static
		EventQueue *GetEventQueue();

		/// whether this module uses an eventqueue ( used only from TickControl )
		bool EventQueueEnabled();

		/// return the ID of the Eventqueue
		size_t GetQueueID();

		/**
			Immediatly send an Event to all (local) listeners.
		*/
		void PostEvent(Event &e);

		/**
			Add an Event to the Eventqueue to be fired on the next frame.
			@param e the Event to send
			@param global if set to true the event will also be sent to all other modules
		*/
		void QueueEvent(const Event &e, bool global = false);

		/**
			Add an Event to the Eventqueue to be fired on the next frame.
			@param EvtName the name of the Event to send
			@param global if set to true the event will also be sent to all other modules
		*/
		void QueueEvent(const std::string &EvtName, bool global = false);

		template<typename EventDataType, typename... Args>
		void QueueEvent(const std::string &Name, bool global, Args &&... args) {
			QueueEvent(Event(Name, EventDataType(args...)), global);
		};

		template<typename EventDataType, typename... Args>
		void QueueEvent(const HashType Hash, bool global, Args &&... args) {
			QueueEvent(Event(Hash, EventDataType(std::forward<Args>(args)...)), global);
		};

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
		static constexpr auto makeEventDef(T t) {
			return EventDef<H, T>(t);
		};

		/**
			Send a key/value pair as debugging information.
			This function is intended to send per frame information to be
			displayed somewhere.
			E.g. Frames per Second, Events per second or some game related
			information
			Sends a DbgStringEvent
		*/
		void DebugString(const std::string &name, const std::string &value);

		/// access this modules boost::thread instance
		boost::thread *getThread();

		/// request this module to quit
		void RequestQuit();

		/// returns the time this module is already running
		sf::Time GetModuleTime();

		/**
			Change the target TicksPerSecond at runtime.
			@see TickControl::SetTargetTicksPerSecond
		*/
		void SetTPS(int TPS);

	protected:
		/**
			Set the Event used to send debug messages.
			Those messages contain per Frame information. A module send per
			default log messegas about the EventQueue (events per second) and the
			TickControl.
			Defaults to VIEW_DBG_STRING.
			@see  TickControl::LogModuleStats()
			@see DebugString()
		*/
		void SetDbgStringEvent(std::shared_ptr<Event>());

		/**
			Change the TickEvent at runtime.
			@see TickControl::SetTickEvent
		*/
		void SetTickEvent(std::shared_ptr<Event> TickEvt);

		/** child classes may override this method with their Thread logic.
			The default implementation uses a TickControl to send the TickEvent once a second.
		    It also logs fps.
		*/
		virtual void Execute();

		/**
			Called once the module is started, use for module-specific initialisation code.
		*/
		virtual void LocalInit() { };

		/**
			Called once all modules are started, use for inter-module initialisation code.
		*/
		virtual void Init() { };

		/**
			Called after Execute() returns.

			@warning
				Any code using references to Module::Get() ( e.g. in the EventUser
				class on destruct ) will segfault when called
				in the modules destructor.
				MAKE SURE YOU DESTROY ANYTHING INSIDE A MODULE USING EVENTUSER OR
				Module::Get() IN THE DeInit() METHOD!
				THIS ALSO MEANS THAT YOU CANT HAVE A NON-POINTER MEMBER IN YOUR MODULE
				CLASS WHICH USES ONE OF THOSE.
				Pointers or std::shared_ptr have to be deleted here in DeInit().
		*/
		virtual void DeInit() { };

		/// Thread-entry point (internal)
		void ThreadLocalInit();

	private:
		friend class GameBase;

		static std::shared_ptr<boost::barrier> &GetBarrier();

		class Private;

		Private *pimpl;
	};

} // namespace sbe

typedef sbe::Module Module;

#endif // MODULE_H
