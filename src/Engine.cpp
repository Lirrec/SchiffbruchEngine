#include "sbe/Engine.hpp"
#include "sbe/Logger.hpp"

#include "sbe/ResourceManager.hpp"
#include "sbe/io/IO.hpp"
#include "sbe/Config.hpp"
#include "sbe/Sound.hpp"
#include "sbe/gfx/Actor.hpp"

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

	Engine::out(Engine::INFO) << "[Engine] ResourceManager..." << std::endl;
	ResMgr.reset 	( new ResourceManager );

	Engine::out(Engine::INFO) << "[Engine] IO..." << std::endl;
	Io.reset 	( new IO );

	ResMgr->init();

	Engine::out(Engine::INFO) << "[Engine] Config..." << std::endl;
	_config.reset( new Config );

	Engine::out(Engine::INFO) << "[Engine] SoundSys..." << std::endl;
	SndSys.reset 	( new SoundSystem );
}

void Engine::UnloadSubSystems()
{
	SndSys  .reset();
	ResMgr  .reset();
	_config.reset();

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
		*(Instance->SpamLogger) << Clock::now().time_since_epoch().count() << "[S]";
		return *(Instance->SpamLogger);
		break;

		case LogLevel::INFO:
		*(Instance->SpamLogger) << Clock::now().time_since_epoch().count() << "[I]";
		return *(Instance->InfoLogger);
		break;

		case LogLevel::WARNING:
		*(Instance->SpamLogger) << Clock::now().time_since_epoch().count() << "[W]";
		return *(Instance->WarningLogger);
		break;

		case LogLevel::ERROR:
		*(Instance->SpamLogger) << Clock::now().time_since_epoch().count() << "[E]";
		return *(Instance->ErrorLogger);
		break;

		default:
			*(Instance->SpamLogger) << Clock::now().time_since_epoch().count() << "[S]";
			return *(Instance->SpamLogger);
			break;
	}
}

std::shared_ptr<SoundSystem>     Engine::GetSndSys() { return Instance->SndSys; }
std::shared_ptr<ResourceManager> Engine::GetResMgr() { return Instance->ResMgr; }
std::shared_ptr<IO>	             Engine::GetIO()     { return Instance->Io; }
std::shared_ptr<Config>	         Engine::getCfg()    { return Instance->_config; }
sf::RenderWindow&                Engine::GetApp()    { return Instance->App; }
