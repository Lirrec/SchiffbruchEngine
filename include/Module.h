#ifndef MODULE_H
#define MODULE_H

#include <boost/thread.hpp>
#include "event/EventQueue.h"

#include <string>
#include <forward_list>

class TickControl;

//contains necessary Information to start a module
struct ModuleStartInfo
{

	ModuleStartInfo()
	 : Name("_NameNotSet"),
		desiredTicksPerSecond(100),
		delay(0),
		useEventQueue(false)
	{
		TickEvt = std::shared_ptr<Event>();
	}

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
	/// How many times per second should this module receive its
	/// tickevent. 0 means no Tickevents will be sent.
	int desiredTicksPerSecond;
	/// Which event should be sent every tick to the module
	std::shared_ptr<Event> TickEvt;

	/// Wether this modules need some time to initialize
	/// 0 means no delay, everything else delay milliseconds
	int delay;

	/// Does the Modules require an event queue
	bool useEventQueue;
};

/** A Game Module which runs in its own Thread.
Has its own eventqueue an registers on run with the global EventCore
*/
class Module : sf::NonCopyable
{
	public:
		Module();
		virtual ~Module();

		static Module* Get() { return Module::Instance.get(); }

		static boost::uuids::uuid NewUUID()
		{
			boost::lock_guard<boost::mutex> lock(UUIDsMutex);
			return  boost::uuids::random_generator()();
		}

		virtual void StartModule( const ModuleStartInfo& m );

		/// returns the modules name
		const std::string& GetName() const { return Name; }

		// TODO: implement routing
		//virtual std::forward_list<std::string> ReturnDesiredEvents() = 0;

		// return evtq non-static
		EventQueue* GetEventQueue() { return EvtQ.get(); };

		size_t GetQueueID() { return QueueID; }

		void PostEvent( Event &e)
		{
			EvtQ->PostEvent( e );
		}

		void QueueEvent( const Event& e, bool global = false)
		{
			EvtQ->QueueEvent( e, global );
		}

		void QueueEvent( const std::string& EvtName, bool global = false)
		{
			EvtQ->QueueEvent( EvtName, global );
		}

		///  add a key/value pair to the debug view
		void DebugString( const std::string& name, const std::string& value);

		/// access this modules boost::thread instance
		boost::thread* getThread() { return MyThread; }

		/// request this module to quit
		void RequestQuit() { quit = true; }

		/// returns the time this module is already running
		sf::Time GetModuleTime() { return ModuleTime.getElapsedTime(); }

	protected:



		std::shared_ptr<Event> DbgStringEvent;

		/// change the target TicksPerSecond at runtime
		void SetTPS( int TPS );
		/// change the TickEvent at runtime
		void SetTickEvent( std::shared_ptr<Event> TickEvt );

		// Init Thread-local Data
		void ThreadLocalInit();

		/// child classes may override this method with their Thread logic
		/// the default implementation runs the eventqueue at the set fps
		/// and reports status once a second
		virtual void Execute();

		virtual void Init() {};
		virtual void DeInit() {};

	private:

		sf::Clock ModuleTime;

		static boost::thread_specific_ptr<Module> Instance;
		static boost::mutex ModulesMutex;
		static boost::mutex UUIDsMutex;
		static std::list< Module* > RunningModules;

		bool useEventQueue;

		friend class TickControl;
		std::shared_ptr<TickControl> TC;
		bool quit;

		// those will be registered and set by Run()
		size_t QueueID;
		std::shared_ptr<EventQueue> EvtQ;

		std::string Name;
		boost::thread* MyThread;
};


#endif // MODULE_H

