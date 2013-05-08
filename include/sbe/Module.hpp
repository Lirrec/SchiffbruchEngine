#ifndef MODULE_H
#define MODULE_H

#include <boost/thread.hpp>

#include <SFML/System/Clock.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <memory>
#include <string>
#include <list>

namespace boost { class thread; }

namespace sbe
{
	class TickControl;
	class EventQueue;
	class Event;

	/**
		This class contains configuration parameters for the behaviour and default settings of a module.
		It provides a clear way to set all required parameters to run a module.
		@see GameBase::RegisterModule()
	*/
	struct ModuleStartInfo
	{

		/**
			Default Constructor.
			The configuration will be as following:
			Name: "_NameNotSet"
			TicksPerSecond: 100
			TickEvent: none
			delay: 0
			useEventQueue: false
		*/
		ModuleStartInfo()
		 : Name("_NameNotSet"),
			desiredTicksPerSecond(100),
			delay(0),
			useEventQueue(true)
		{
			TickEvt = std::shared_ptr<Event>();
		}

		/** Verbose Constructor.
			provides all parameters in the constructor with default-values
		*/
		ModuleStartInfo(std::string name,
					int tps = 100,
					std::shared_ptr<Event> Evt = std::shared_ptr<Event>(),
					int _delay = 0,
					bool useQueue = true)
		 : Name(name),
		   desiredTicksPerSecond(tps),
		   TickEvt(Evt),
		   delay(_delay),
		   useEventQueue(useQueue)
		{}

		/// The name of the module to create
		std::string Name;
		/** @see Module::SetTPS()
		 */
		int desiredTicksPerSecond;
		/// @see Module::SetTickEvent()
		std::shared_ptr<Event> TickEvt;

		/** Specify a delay to wait after this modules thread was started.
			0 means no delay
		*/
		int delay;

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
					MAKE SURE YOU DESTROY ANYTHING INSIDE A MODULE USING EVENTUSER OR Module::Get() IN THE DeInit() METHOD!

		Modules have their own local eventqueue and register on start with the global EventCore.
	*/
	class Module : sf::NonCopyable
	{
		public:
			Module();
			virtual ~Module();

			/// Static module getter. Use this in your code to access the current module.
			static Module* Get() { return Module::Instance.get(); }

			///	Starts the module thread
			virtual void StartModule( const ModuleStartInfo& m );

			/// returns the modules name
			const std::string& GetName() const { return Name; }

			// TODO: implement routing
			//virtual std::forward_list<std::string> ReturnDesiredEvents() = 0;

			/// return evtq non-static
			EventQueue* GetEventQueue();

			/// return the ID of the Eventqueue
			size_t GetQueueID() { return QueueID; }

			/**
				Immediatly send an Event to all (local) listeners.
			*/
			void PostEvent( Event &e );

			/**
				Add an Event to the Eventqueue to be fired on the next frame.
				@param e the Event to send
				@param global if set to true the event will also be sent to all other modules
			*/
			void QueueEvent( const Event& e, bool global = false);

			/**
				Add an Event to the Eventqueue to be fired on the next frame.
				@param EvtName the name of the Event to send
				@param global if set to true the event will also be sent to all other modules
			*/
			void QueueEvent( const std::string& EvtName, bool global = false);

			/**
				Send a key/value pair as debugging information.
				This function is intendet to send per frame information to be displayed somewhere.
				E.g. Frames per Second, Events per second or some game related information
				Sends a DbgStringEvent
			*/
			void DebugString( const std::string& name, const std::string& value);

			/// access this modules boost::thread instance
			boost::thread* getThread() { return MyThread; }

			/// request this module to quit
			void RequestQuit() { quit = true; }

			/// returns the time this module is already running
			sf::Time GetModuleTime() { return ModuleTime.getElapsedTime(); }

			/**
				Change the target TicksPerSecond at runtime.
				@see TickControl::SetTargetTicksPerSecond
			*/
			void SetTPS( int TPS );

		protected:

			/**
				Set the Event used to send debug messages.
				Those messages contain per Frame information. A module send per default log messegas about the EventQueue (events per second) and the TickControl.
				Defaults to VIEW_DBG_STRING.
				@see  TickControl::LogModuleStats()
				@see DebugString()
			*/
			void SetDbgStringEvent( std::shared_ptr<Event>() );


			/**
				Change the TickEvent at runtime.
				@see TickControl::SetTickEvent
			*/
			void SetTickEvent( std::shared_ptr<Event> TickEvt );

			/** child classes may override this method with their Thread logic.
				The default implementation uses a TickControl to send the TickEvent once a second. It also logs fps.
			*/
			virtual void Execute();

			/**
				Called once the module is started, use for module-specific initialisation code.
			*/
			virtual void Init() {};
			/**
				Called after Execute() returns.

				@warning
				Any code using references to Module::Get() ( e.g. in the EventUser class on destruct ) will segfault when called
				in the modules destructor.
				MAKE SURE YOU DESTROY ANYTHING INSIDE A MODULE USING EVENTUSER OR Module::Get() IN THE DeInit() METHOD!
				THIS ALSO MEANS THAT YOU CANT HAVE A NON-POINTER MEMBER IN YOUR MODULE CLASS WHICH USES ONE OF THOSE.
				Pointers or std::shared_ptr have to be deleted here in DeInit().
			*/
			virtual void DeInit() {};

			/// Thread-entry point (internal)
			void ThreadLocalInit();

		private:

			/// Module internal timer
			sf::Clock ModuleTime;

			/// Event sent with key/value debbuging information
			std::shared_ptr<Event> DbgStringEvent;

			/// ptr to module instance
			static boost::thread_specific_ptr<Module> Instance;
			/// mutex for module access
			static boost::mutex ModulesMutex;

			/// List of existing Modules
			static std::list< Module* > RunningModules;

			/// True if the Modules needs an EventQueue
			bool useEventQueue;


			friend class TickControl;
			/// this modules TickControl
			std::shared_ptr<TickControl> TC;
			/// Eventloop termination condition
			bool quit;

			/// those will be registered and set by Run()
			size_t QueueID;
			/// this modules EventQueue
			std::shared_ptr<EventQueue> EvtQ;

			/// Module name
			std::string Name;
			/// Lokal thread pointer
			boost::thread* MyThread;
	};

} // namespace sbe

typedef sbe::Module Module;

#endif // MODULE_H
