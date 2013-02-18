#include "sbe/GameBase.hpp"

#include <SFML/Graphics.hpp>

#include <boost/thread.hpp>

#include "sbe/Module.hpp"
#include "sbe/Engine.hpp"

#include "modules/Core.hpp"

#include <memory>
#include <iostream>

GameBase::GameBase()
{
}

void GameBase::Run()
{
	EngineInit();

	ModuleInit();

	GameInit();

	StartModules();
	JoinModules();

	CleanUp();
}

void GameBase::EngineInit()
{
	/// Engine
	_Engine.reset( new Engine() );

	_Engine->CreateSubSystems();

	// EventSystem
	ModuleStartInfo CoreInfo;
	CoreInfo.Name = "EventCore";
	CoreInfo.useEventQueue = false;

	RegisterModule(new Core, CoreInfo);



	Engine::out(Engine::INFO) << "[Engine] Initialisation done..." << std::endl;
}


void GameBase::CleanUp()
{
	Engine::out(Engine::INFO) << "[Engine] Starting Cleanup ... " << std::endl ;

	Engine::out(Engine::INFO) << "[Engine] Deleting Modules ... " << std::endl ;
	for ( auto r_it = Modules.rbegin(); r_it != Modules.rend(); ++r_it)
	{
		Engine::out() << r_it->first << " - " << r_it->second.Name << std::endl;
		delete r_it->first;
	}
	Modules.clear();


	Engine::out(Engine::INFO) << "[Engine] Deleting Game ... " << std::endl ;
	DeInit();

	Engine::out(Engine::INFO) << "[Engine] Deleting Engine ... " << std::endl ;
	_Engine.reset();

}


void GameBase::RegisterModule(Module* Mod, const ModuleStartInfo& Info)
{
	Engine::out(Engine::INFO) << "[Engine] New Module: [" << Info.Name << "]" << std::endl;
	Modules.push_back(std::make_pair(Mod, Info));
}

void GameBase::StartModules()
{
	Engine::out(Engine::INFO) << "[Engine] Starting Modules: " << std::endl;

	for (auto& M : Modules)
	{
		//Engine::out() << "[" << M.second.Name << "]" << std::endl;
		M.first->StartModule(M.second);

		if (M.second.delay != 0) boost::this_thread::sleep(boost::posix_time::milliseconds(M.second.delay));
	}
}

void GameBase::JoinModules()
{
	Engine::out(Engine::INFO) << "[Engine] Joining Threads." << std::endl;

	for ( auto r_it = Modules.rbegin(); r_it != Modules.rend(); ++r_it)
	{
		if (!r_it->first->getThread()->joinable() ) Engine::out(Engine::ERROR) << "[Engine] ERROR: can't join Threads!!" << std::endl;

		r_it->first->getThread()->join();
	}
}
