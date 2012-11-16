#include "sbe/ResourceManager.hpp"

ResourceManager::ResourceManager()
{
}

void ResourceManager::init()
{
    RegisterClass<sf::Font>();
    RegisterClass<sf::Texture>();
    RegisterClass<sf::SoundBuffer>();
    RegisterClass<ImageSet>();

	sf::Font* f = new sf::Font(getDefaultFont());
	add( f, "default");
}

ResourceManager::~ResourceManager()
{

	DumpDebugInfo();
}

void ResourceManager::DumpDebugInfo()
{

	Engine::out() << "[ResourceManager] Resources Overview:" << std::endl;
    for (auto &it : Resources)
    {
        Engine::out() << "Class: " << it.first.name() << std::endl;
        boost::any_cast<NamedList<boost::any>>(it).DebugDump();
    }

	Engine::out() << "[ResourceManager] done." << std::endl;
}

const sf::Font& ResourceManager::getDefaultFont(){
	static sf::Font font;
	static bool loaded = false;

	if(!loaded){
		static const signed char data[] = {
			#include "Arial.hpp"
		};
		font.loadFromMemory(data, sizeof(data));
		loaded = true;
	}
	return font;
}
