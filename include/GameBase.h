#ifndef GAMEBASE_H
#define GAMEBASE_H

#include <SFML/System.hpp>
#include <boost/thread.hpp>

class Logger;
class Module;
class ModuleStartInfo;
class Engine;


#include <memory>
#include <vector>
#include <ostream>

class GameBase
{
	public:

		GameBase();

		void RegisterModule( Module* Mod, const ModuleStartInfo& Info );

		/// Start the mainloop
		void Run();

		/// Create a new Game
		/// The Game should initialise its own classes here, called after the engine has been started
		virtual void GameInit() = 0;

		///
		///
		virtual void DeInit() = 0;

		/// The Game should call RegisterModule with every module it wants to be started
		virtual void ModuleInit() = 0;

	private:

		/// Initialize the Engine
		void EngineInit();

		/// start all registered Modules
		void StartModules();
		void JoinModules();

		/// Cleanup and delete all engine Objects
		void CleanUp();

		std::shared_ptr<Engine> _Engine;

		std::vector<std::pair<Module*, ModuleStartInfo>> Modules;
};


#endif // GAMEBASE_H

