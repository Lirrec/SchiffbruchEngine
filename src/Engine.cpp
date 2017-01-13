#include "sbe/Engine.hpp"


#include "sbe/Config.hpp"
#include "sbe/Sound.hpp"
#include "sbe/gfx/Actor.hpp"

#include "sbe/event/Event.hpp"
#include "sbe/event/SFMLEvent.hpp"

#include "sbe/io/IO.hpp"
#include "sbe/ResourceManager.hpp"
#include "sbe/util/console/CommandParser.hpp"

#include "sbe/entity/EntityManager.hpp"

#include <chrono>

namespace sbe
{

	typedef std::chrono::high_resolution_clock Clock;

	Engine* Engine::Instance;

	Engine::Engine() {
		Instance = this;
	}

	Engine::~Engine() {
		UnloadSubSystems();
	}

	void Engine::CreateSubSystems() {
		// Logging
		std::cout << "[Engine] Logger." << std::endl;
		SpamLogger = std::make_shared<Logger>(std::cout);
		InfoLogger = std::make_shared<Logger>(std::cout);
		WarningLogger = std::make_shared<Logger>(std::cout);
		ErrorLogger = std::make_shared<Logger>(std::cout);

		Engine::out(Engine::INFO) << "[Engine] ResourceManager..." << std::endl;
		ResMgr = std::make_shared<ResourceManager>();

		Engine::out(Engine::INFO) << "[Engine] IO..." << std::endl;
		Io = std::make_shared<IO>();

		ResMgr->init();

		Engine::out(Engine::INFO) << "[Engine] CommandParser..." << std::endl;
		CmdPrs = std::make_shared<CommandParser>();

		Engine::out(Engine::INFO) << "[Engine] Config..." << std::endl;
		_config = std::make_shared<Config>();

		Engine::out(Engine::INFO) << "[Engine] SoundSys..." << std::endl;
		SndSys = std::make_shared<SoundSystem>();

		Engine::out(Engine::INFO) << "[Engine] EntityManager..." << std::endl;
		EntityMgr = std::make_shared<EntityManager>();

		Engine::out(Engine::INFO) << "[Engine] Window..." << std::endl;
		App = std::make_shared<sf::RenderWindow>();
	}

	void Engine::UnloadSubSystems() {
		EntityMgr.reset();
		SndSys.reset();
		ResMgr.reset();
		_config.reset();
		CmdPrs.reset();

		SpamLogger.reset();
		InfoLogger.reset();
		WarningLogger.reset();
		ErrorLogger.reset();
	}

	std::string Engine::getTimeStamp() {
		std::stringstream ts;

		char timestamp[128];
		auto time = Clock::now();
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch());
		auto fraction = time - seconds;
		auto fr_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction.time_since_epoch());
		auto time_t = std::chrono::system_clock::to_time_t(time);
		if (std::strftime(timestamp, sizeof(timestamp), "%d.%m.%Y %H:%M:%S", std::localtime(&time_t)) == 0)
		{
			*(Instance->ErrorLogger) << "Unable to create Timestamp!" << std::endl;
		}
		ts << timestamp << "." << fr_milliseconds.count();
		return ts.str();
	}

	Logger& Engine::out(LogLevel level) {
		switch (level)
		{
			case LogLevel::SPAM:
				*(Instance->SpamLogger) << getTimeStamp() << " [S]";
				return *(Instance->SpamLogger);
				break;

			case LogLevel::INFO:
				*(Instance->SpamLogger) << getTimeStamp() << " [I]";
				return *(Instance->InfoLogger);
				break;

			case LogLevel::WARNING:
				*(Instance->SpamLogger) << getTimeStamp() << " [W]";
				return *(Instance->WarningLogger);
				break;

			case LogLevel::ERROR:
				*(Instance->SpamLogger) << getTimeStamp() << " [E]";
				return *(Instance->ErrorLogger);
				break;

			default:
				*(Instance->SpamLogger) << getTimeStamp() << " [S]";
				return *(Instance->SpamLogger);
				break;
		}
	}

	std::shared_ptr<SoundSystem>     Engine::GetSndSys() { return Instance->SndSys; }

	std::shared_ptr<ResourceManager> Engine::GetResMgr() { return Instance->ResMgr; }

	std::shared_ptr<EntityManager> Engine::GetEntityMgr() { return Instance->EntityMgr; }

	std::shared_ptr<IO>                 Engine::GetIO() { return Instance->Io; }

	std::shared_ptr<Config>             Engine::getCfg() { return Instance->_config; }

	std::shared_ptr<CommandParser>   Engine::GetCmdParser() { return Instance->CmdPrs; }

	sf::RenderWindow& Engine::GetApp() { return *(Instance->App); }

} // namespace sbe
