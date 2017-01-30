#include <sbe/GameBase.hpp>

#include <SFML/Graphics.hpp>

#include <boost/thread.hpp>
#include <sbe/util/SimpleModule.hpp>
#include <event/EventCore.hpp>

#include <sbe/Module.hpp>
#include <sbe/Engine.hpp>


namespace sbe
{

	GameBase::GameBase() {
	}

	void GameBase::Run() {
		EngineInit();

		ModuleInit();

		GameInit();

		StartModules();
		JoinModules();

		CleanUp();
	}

	void GameBase::EngineInit() {
		/// Engine
		_Engine = std::make_shared<Engine>();

		_Engine->CreateSubSystems();

		EventsInit();

		Engine::out(Engine::INFO) << "[Engine] Initialisation done..." << std::endl;
	}

	void GameBase::EventsInit() {
		// Singleton for the EventCore (leaked, destroyed on program shutdown )
		auto myEventCore = new EventCore();

		// Start a Module without EventQueue
		// this module will be the host-module which executes EventCore Ticks
		// the logic for this in in TickControl, so we can create a completely empty no-op module
		ModuleStartInfo CoreInfo;
		CoreInfo.Name = "EventCore";
		CoreInfo.useEventQueue = false;
		RegisterModule(new SimpleModule<>, CoreInfo);
	}


	void GameBase::CleanUp() {
		Engine::out(Engine::INFO) << "[Engine] Starting Cleanup ... " << std::endl;

		Engine::out(Engine::INFO) << "[Engine] Deleting Modules ... " << std::endl;
		for (auto r_it = Modules.rbegin(); r_it != Modules.rend(); ++r_it)
		{
			Engine::out() << r_it->first << " - " << r_it->second.Name << std::endl;
			delete r_it->first;
		}
		Modules.clear();


		Engine::out(Engine::INFO) << "[Engine] Deleting Game ... " << std::endl;
		DeInit();

		Engine::out(Engine::INFO) << "[Engine] Deleting Engine ... " << std::endl;
		_Engine.reset();

	}


	void GameBase::RegisterModule(Module* Mod, const ModuleStartInfo& Info) {
		Engine::out(Engine::INFO) << "[Engine] New Module: [" << Info.Name << "]" << std::endl;
		Modules.push_back(std::make_pair(Mod, Info));
	}

	void GameBase::StartModules() {
		Engine::out(Engine::INFO) << "[Engine] Starting Modules: " << std::endl;

		// the count on the barrier is Modules.size() + 1 for this thread
		Module::GetBarrier() = std::make_shared<boost::barrier>(Modules.size() + 1);

		for (auto& M : Modules)
		{
			//Engine::out() << "[" << M.second.Name << "]" << std::endl;
			M.first->StartModule(M.second);
		}

		Module::GetBarrier()->wait();
	}

	void GameBase::JoinModules() {
		Engine::out(Engine::INFO) << "[Engine] Joining Threads." << std::endl;

		for (auto r_it = Modules.rbegin(); r_it != Modules.rend(); ++r_it)
		{
			if (!r_it->first->getThread()->joinable()) Engine::out(Engine::ERROR) << "[Engine] ERROR: can't join Threads!!" << std::endl;
			r_it->first->getThread()->join();
		}
	}

} // namespace sbe
