#include "sbe/ResourceManager.hpp"

ResourceManager::ResourceManager()
 : usedmemory(0)
{
}

ResourceManager::init()
{
    RegisterClass<sf::Font>();
    RegisterClass<sf::Texture>();
    RegisterClass<sf::SoundBuffer>();
    RegisterClass<ImageSet>();

	sf::Font* f = new sf::Font(getDefaultFont());
	AddItem("default", f );
}

ResourceManager::~ResourceManager()
{

	DumpDebugInfo();
}

void ResourceManager::DumpDebugInfo()
{

	Engine::out() << "[ResourceManager] Resources Overview:" << std::endl;
    for (auto it : Resources)
    {
        Engine::out() << "Class: " << it->first.name() << std::endl;
        it->DebugDump();
    }

	Engine::out() << "[ResourceManager] done." << std::endl;
}

