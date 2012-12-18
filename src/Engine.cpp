#include "sbe/Engine.hpp"
#include "sbe/Logger.hpp"

#include "sbe/ResourceManager.hpp"
#include "sbe/io/IO.hpp"
#include "sbe/Sound.hpp"
#include "sbe/Graphic.hpp"

#include "sbe/event/Event.hpp"
#include "sbe/event/SFMLEvent.hpp"

#include <memory>
#include <iostream>



Engine* Engine::Instance;

Engine::Engine()
{
	Instance = this;
}

Engine::~Engine()
{
	UnloadSubSystems();
}

void Engine::CreateSubSystems()
{
	// Logging
	std::cout << "[Engine] Logger." << std::endl;
	Log.reset( new Logger );

	Engine::out() << "[Engine] ResourceManager..." << std::endl;
	ResMgr.reset 	( new ResourceManager );
	ResMgr->init();
	Engine::out() << "[Engine] SoundSys..." << std::endl;
	SndSys.reset 	( new SoundSystem );
}

void Engine::UnloadSubSystems()
{
	SndSys  .reset();
	ResMgr  .reset();

	Log.reset();
}

Logger& Engine::out() { return *(Instance->Log); }

std::shared_ptr<SoundSystem> 		Engine::GetSndSys() 		{ return Instance->SndSys; }
std::shared_ptr<ResourceManager>	Engine::GetResMgr() 		{ return Instance->ResMgr; }
std::shared_ptr<IO>	Engine::GetIO() 		{ return Instance->Io; }
sf::RenderWindow&					Engine::GetApp() 		{ return Instance->App; }
