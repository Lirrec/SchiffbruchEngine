#include "sbe/ResourceManager.hpp"

ResourceManager::ResourceManager()
 : usedmemory(0)
{


}

void ResourceManager::init()
{
    RegisterClass<sf::Font>();
    RegisterClass<sf::Texture>();
    RegisterClass<sf::SoundBuffer>();
    RegisterClass<ImageSet>();


    // load default font
	// sadly, the built-in default Font of sfml got deleted,
	// so we've got to build-in it ourselves...
	const signed char data[] = { // 'stole' the Arial.hpp from earlier sfml
		#include "Arial.hpp"
	};


	sf::Font f;
    f.loadFromMemory(data, sizeof(data));

	AddItem("default", f );
}

ResourceManager::~ResourceManager()
{

	DumpDebugInfo();
}

void ResourceManager::DumpDebugInfo()
{

	Engine::out() << "# Resources Overview #" << std::endl;
    for (auto it : Resources)
    {
        Engine::out() << "Class: " << it->first.name() << std::endl;
        it->DebugDump();
    }

	Engine::out() << "# ------------------ #" << std::endl;
}

