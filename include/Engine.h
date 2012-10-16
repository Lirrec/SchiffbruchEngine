#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>

#include <boost/thread.hpp>
#include "Logger.h"

class SoundSystem;
class ResourceManager;
class Graphics;
class AssetManager;
class SFMLEventConverter;

namespace UI { class Screen; }

//class Logger;

#include <memory>



class Engine
{
	public:

		Engine();
		~Engine();

		void CreateSubSystems();
		void UnloadSubSystems();

		static std::shared_ptr<Logger> GetLogger() { return Instance->Log; }
		// returns a reference to the currently valid output and logging stream
		// (could be to stdout or some class )
		static Logger& out();

		static std::shared_ptr<SoundSystem> 		GetSndSys ();
		static std::shared_ptr<ResourceManager>		GetResMgr ();
		static sf::RenderWindow&					GetApp    ();


	private:



		sf::RenderWindow App;

		static Engine* Instance;
		std::shared_ptr<Logger>			Log;

		std::shared_ptr<SoundSystem> 	SndSys;
		std::shared_ptr<ResourceManager>	ResMgr;
};


#endif // ENGINE_H
