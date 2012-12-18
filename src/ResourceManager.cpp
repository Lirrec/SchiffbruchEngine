#include "sbe/ResourceManager.hpp"

#include "sbe/io/IOPlugin.hpp"
#include "io/plugins/ImageSetIOPlugin.hpp"
#include "io/plugins/SoundIOPlugin.hpp"
#include "io/plugins/ImageIOPlugin.hpp"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

    DumpDebugInfo();
}

void ResourceManager::init()
{
	Engine::out() << "[ResourceManager] Creating IO" << std::endl;
	mIO.reset( new IO() );

	loadDefaultPlugins();

	Engine::out() << "[ResourceManager] Adding default font" << std::endl;
    sf::Font* f = new sf::Font(getDefaultFont());
    add( f, "default");
}




void ResourceManager::loadDefaultPlugins()
{
	Engine::out() << "[ResourceManager] Loading IO plugins:" << std::endl;

	std::shared_ptr<IOPlugin> SoundIOP ( new SoundIOPlugin() );
	registerResource<sf::SoundBuffer>( iResource::createResInfo("SoundBuffer", false, false), SoundIOP);

	std::shared_ptr<IOPlugin> ImgIOP ( new ImageIOPlugin() );
	registerResource<sf::Image>( iResource::createResInfo("Image", false, false), ImgIOP);

	std::shared_ptr<IOPlugin> ImageSetIOP ( new ImageSetIOPlugin() );
	registerResource<ImageSet>( iResource::createResInfo("ImageSet", false, false), ImageSetIOP);

	registerResource<sf::Font>( "Font" );

	Engine::out() << "[ResourceManager] IO Plugins loaded." << std::endl;
}


void ResourceManager::DumpDebugInfo()
{

    Engine::out() << "[ResourceManager] Resources Overview:" << std::endl;
    for (auto &it : Resources)
    {
        Engine::out() << "Class: " << ResInfos[it.first].name << std::endl;

        (it.second)->DebugDump();
    }

    Engine::out() << "[ResourceManager] done." << std::endl;
}

const sf::Font& ResourceManager::getDefaultFont()
{
    static sf::Font font;
    static bool loaded = false;

    if(!loaded)
    {
        static const signed char data[] =
        {
#include "Arial.hpp"
        };
        font.loadFromMemory(data, sizeof(data));
        loaded = true;
    }
    return font;
}
