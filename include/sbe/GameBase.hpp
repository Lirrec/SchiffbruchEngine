#ifndef GAMEBASE_H
#define GAMEBASE_H

#include <memory>
#include <vector>
#include <ostream>

namespace sbe
{

	class Logger;

	class Module;

	struct ModuleStartInfo;

	class Engine;

	/**
		This is the base class one should inherit for using SBE.

		To use SBE one should follow these steps:
		- Create the Modules which represent your Game. @see Module
		- Inherit from GameBase and implement the virtual functions provided:
			- GameInit		- (optional) - you can run some initialisation code here ( or in each module )
			- DeInit		- (optional) - you can run some cleanup code here
			- ModuleInit	- (required) - Use RegisterModule() to tell the engine about your modules

		@sa Module, Event, Engine
	*/
	class GameBase
	{
	public:

		GameBase();


		/// Starts the mainloop
		void Run();

	protected:

		/**
			Register a module to be started.
			@see Module
		*/
		void RegisterModule(Module* Mod, const ModuleStartInfo& Info);

		/// Game related init- The Game should initialize its own classes here, called after the engine and modules have been started
		virtual void GameInit() { };

		/// Game related cleanup - called after all Modules have been unloaded
		virtual void DeInit() { };

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

} // namespace sbe
#endif // GAMEBASE_H

