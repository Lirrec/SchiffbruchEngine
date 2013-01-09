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
	SpamLogger.reset( new Logger );
	InfoLogger.reset( new Logger );
	WarningLogger.reset( new Logger );
	ErrorLogger.reset( new Logger );

	Engine::out() << "[Engine] ResourceManager..." << std::endl;
	ResMgr.reset 	( new ResourceManager );

	Engine::out() << "[Engine] IO..." << std::endl;
	Io.reset 	( new IO );

	ResMgr->init();


	Engine::out() << "[Engine] SoundSys..." << std::endl;
	SndSys.reset 	( new SoundSystem );
}

void Engine::UnloadSubSystems()
{
	SndSys  .reset();
	ResMgr  .reset();

	SpamLogger.reset();
	InfoLogger.reset();
	WarningLogger.reset();
	ErrorLogger.reset();
}

Logger& Engine::out(LogLevel level)
{
	switch ( level )
	{
		case LogLevel::SPAM:
		return *(Instance->SpamLogger);
		break;

		case LogLevel::INFO:
		return *(Instance->InfoLogger);
		break;

		case LogLevel::WARNING:
		return *(Instance->WarningLogger);
		break;

		case LogLevel::ERROR:
		return *(Instance->ErrorLogger);
		break;
	} 
}

std::shared_ptr<SoundSystem> 		Engine::GetSndSys() 		{ return Instance->SndSys; }
std::shared_ptr<ResourceManager>	Engine::GetResMgr() 		{ return Instance->ResMgr; }
std::shared_ptr<IO>	Engine::GetIO() 		{ return Instance->Io; }
sf::RenderWindow&					Engine::GetApp() 		{ return Instance->App; }
