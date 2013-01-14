#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include <boost/thread.hpp>

#include "Logger.hpp"

class SoundSystem;
class ResourceManager;
class Graphics;
class AssetManager;
class SFMLEventConverter;
class IO;
class Config;
//class Logger;

#include <memory>
#include <chrono>



class Engine
{

	public:

	typedef std::chrono::high_resolution_clock Clock;

	enum LogLevel
	{
		SPAM,INFO,WARNING,ERROR
	};

		Engine();
		~Engine();

		void CreateSubSystems();
		void UnloadSubSystems();

		static std::shared_ptr<Logger> GetLogger(LogLevel level = LogLevel::SPAM)
		{
			switch ( level )
			{
				case LogLevel::SPAM:
				return Instance->SpamLogger;
				break;

				case LogLevel::INFO:
				return Instance->InfoLogger;
				break;

				case LogLevel::WARNING:
				return Instance->WarningLogger;
				break;

				case LogLevel::ERROR:
				return Instance->ErrorLogger;
				break;
			}
		}
		// returns a reference to the currently valid output and logging stream
		// (could be to stdout or some class )
		static Logger& out(LogLevel level = LogLevel::INFO);

		static std::shared_ptr<SoundSystem> 	GetSndSys ();
		static std::shared_ptr<ResourceManager> GetResMgr ();
		static sf::RenderWindow&                GetApp    ();
		static std::shared_ptr<IO>              GetIO();
		static std::shared_ptr<Config>          getCfg();


	private:



		sf::RenderWindow App;

		static Engine* Instance;
		std::shared_ptr<Logger>			SpamLogger;
		std::shared_ptr<Logger>			InfoLogger;
		std::shared_ptr<Logger>			WarningLogger;
		std::shared_ptr<Logger>			ErrorLogger;

		std::shared_ptr<SoundSystem> 	SndSys;
		std::shared_ptr<ResourceManager>	ResMgr;
		std::shared_ptr<IO>	Io;
		std::shared_ptr<Config>	_config;
};


#endif // ENGINE_H
