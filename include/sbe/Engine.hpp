#ifndef ENGINE_H
#define ENGINE_H

#include "Logger.hpp"
#include <memory>

namespace sf { class RenderWindow; }

namespace sbe
{

	class SoundSystem;
	class ResourceManager;
	class EntityManager;
	class Graphics;
	class AssetManager;
	class SFMLEventConverter;
	class IO;
	class Config;
	class CommandParser;

	/**
		Main class for the Engine providing static access to most subsystems.

		This class holds pointers to all subsystems and provides an easy interface to the logger.
		Just use the out() method like std::cout. The parameter defines the loglevel/severity of the message.
		Default is LogLevel::SPAM ( not shown in the ingame DebugWindow, only on the console ).
		@code
			Engine::out(Engine::ERROR) << "Error!! " << i << std::endl;
		@endcode

	*/
	class Engine
	{

		public:


		/// Defines the differnt severity levels of log messages
		enum LogLevel
		{
			/// Used for all messages appearing frequently, like debbuging messages for every frame
			SPAM,
			/// General informative message
			INFO,
			/// Warnings
			WARNING,
			/// Errors
			ERROR
		};

			Engine();
			~Engine();

			/// create and initialize Engine subsystems ( called per default only from GameBase )
			void CreateSubSystems();
			/// destroy Engine subsystems ( called per default only from GameBase )
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

				return Instance->SpamLogger;
			}
			/**
				Returns a reference to the currently valid output and logging stream.
				(could be to stdout or some class )
				@param level the severity of the log message
				@see Engine::LogLevel
			*/
			static Logger& out(LogLevel level = LogLevel::SPAM);

			/// static access to the SoundSystem
			static std::shared_ptr<SoundSystem> 	GetSndSys ();
			/// static access to the ResourceManager
			static std::shared_ptr<ResourceManager> GetResMgr ();
			/// static access to the EntityManager
			static std::shared_ptr<EntityManager> GetEntityMgr ();
			/// static access to the sf::RenderWindow
			static sf::RenderWindow&                GetApp    ();
			/// static access to the IO
			static std::shared_ptr<IO>              GetIO();
			/// static access to the Config
			static std::shared_ptr<Config>          getCfg();
			/// static access to the CommandParser
			static std::shared_ptr<CommandParser>   GetCmdParser();


		private:
			std::shared_ptr<sf::RenderWindow> App;

			static Engine* Instance;
			std::shared_ptr<Logger>			SpamLogger;
			std::shared_ptr<Logger>			InfoLogger;
			std::shared_ptr<Logger>			WarningLogger;
			std::shared_ptr<Logger>			ErrorLogger;

			std::shared_ptr<SoundSystem>     SndSys;
			std::shared_ptr<ResourceManager> ResMgr;
			std::shared_ptr<EntityManager> EntityMgr;
			std::shared_ptr<IO>              Io;
			std::shared_ptr<Config>	_config;
			std::shared_ptr<CommandParser>   CmdPrs;
	};

} // namespace sbe

typedef sbe::Engine Engine;

#endif // ENGINE_H
