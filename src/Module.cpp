#include "sbe/Module.hpp"

#include <boost/thread.hpp>

// local includes
#include "event/EventCore.hpp"
#include "modules/Core.hpp"
#include "util/TickControl.hpp"


namespace sbe
{


	class Module::Private {

			friend class Module;
		private:
			/// Module internal timer
			sf::Clock ModuleTime;

			/// Event sent with key/value debbuging information
			std::shared_ptr<Event> DbgStringEvent;

			/// ptr to module instance
			static boost::thread_specific_ptr<Module> Instance;
			/// mutex for module access
			static boost::mutex ModulesMutex;

			/// Lokal thread pointer
			boost::thread* MyThread;

			/// barrier for synchronisation ( initialisation )
			static std::shared_ptr<boost::barrier> ModulesBarrier;

			friend class GameBase;
			/// GameBase needs access to the ModulesBarrier.

			/// List of existing Modules
			static std::list<Module*> RunningModules;

			/// True if the Modules needs an EventQueue
			bool useEventQueue;


			friend class TickControl;

			/// this modules TickControl
			std::shared_ptr<TickControl> TC;
			/// Eventloop termination condition
			bool quit;

			/// those will be registered and set by Run()
			size_t QueueID;
			/// this module's EventQueue
			std::shared_ptr<EventQueue> EvtQ;

			/// Module name
			std::string Name;

	};



	boost::thread_specific_ptr<Module>  Module::Private::Instance;
	std::list<Module*>                  Module::Private::RunningModules;
	boost::mutex                        Module::Private::ModulesMutex;
	std::shared_ptr<boost::barrier>     Module::Private::ModulesBarrier;



	Module::Module() : pimpl(new Private) {
		//Engine::out() << "Module::Module():  this [" << this <<  "]" << std::endl;
	}

	Module::~Module() {
		delete pimpl;
		// make sure this pointer doesnt try do delete 'this' after destructor
		//ThreadModule.release();
	}


	std::shared_ptr<boost::barrier>& Module::GetBarrier() {
		return Private::ModulesBarrier;
	}

	bool Module::EventQueueEnabled() {
		return pimpl->useEventQueue;
	}

	Module* Module::Get() { return Private::Instance.get(); }

	EventQueue *Module::Evt() {
		return Private::Instance.get()->GetEventQueue();
	}

	const std::string& Module::GetName() const { return pimpl->Name; }

	size_t Module::GetQueueID() { return pimpl->QueueID; }


	boost::thread* Module::getThread() { return pimpl->MyThread; }

	void Module::RequestQuit() { pimpl->quit = true; }

	sf::Time Module::GetModuleTime() { return pimpl->ModuleTime.getElapsedTime(); }


	void Module::DebugString(const std::string& name, const std::string& value) {
		pimpl->DbgStringEvent->Data() = std::pair<std::string, std::string>(name, value);

		if (pimpl->useEventQueue)
		{
			QueueEvent(*pimpl->DbgStringEvent, true);
		} else
		{
			// XXX: This is hardcoded!!
			if (pimpl->Name == "EventCore") Core::EvtCore->PostEventToQueue(1, *pimpl->DbgStringEvent);
		}
	}

	//EventQueue&  Module::GetEventQueue()

	void Module::StartModule(const ModuleStartInfo& m)
	{
		pimpl->useEventQueue = m.useEventQueue;

		pimpl->DbgStringEvent.reset(new Event("VIEW_DBG_STRING"));

		pimpl->TC.reset(new TickControl);
		pimpl->Name = m.Name;
		pimpl->quit = false;
		pimpl->TC->Init(m.desiredTicksPerSecond, m.TickEvt);

		if (pimpl->useEventQueue)
		{
			pimpl->EvtQ.reset(new EventQueue());
			pimpl->QueueID = Core::EvtCore->RegisterModule(*this);
		}

		pimpl->MyThread = new boost::thread(boost::bind(&Module::ThreadLocalInit, this));
		//MyThread->detach();
	}

	EventQueue* Module::GetEventQueue() { return pimpl->EvtQ.get(); };

	void Module::PostEvent(Event& e) {
		pimpl->EvtQ->PostEvent(e);
	}

	void Module::QueueEvent(const Event& e, bool global) {
		pimpl->EvtQ->QueueEvent(e, global);
	}

	void Module::QueueEvent(const std::string& EvtName, bool global) {
		pimpl->EvtQ->QueueEvent(EvtName, global);
	}


	void Module::SetTPS(int TPS) {
		pimpl->TC->SetTargetTicksPerSecond(TPS);
	}

	void Module::SetTickEvent(std::shared_ptr<Event> TickEvt) {
		pimpl->TC->SetTickEvent(TickEvt);
	}

	void Module::ThreadLocalInit() {
		pimpl->Instance.reset(this);
		pimpl->ModuleTime.restart();

		Engine::out(Engine::INFO) << "[" << Module::Get()->GetName() << "] New module." << std::endl;

		LocalInit();
		pimpl->ModulesBarrier->wait();
		Init();
		Execute();
		DeInit();

		if (pimpl->useEventQueue) Core::EvtCore->RemoveModule(pimpl->QueueID);
		Engine::out(Engine::INFO) << "[" << Module::Get()->GetName() << "] Thread/Module has exited" << std::endl;
		// without this the thread_specific_ptr would call delete on this module
		pimpl->Instance.release();
	}

	void Module::Execute() {
		while (!pimpl->quit) pimpl->TC->Tick();
	}

} // namespace sbe
