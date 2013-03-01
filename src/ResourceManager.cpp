#include "sbe/ResourceManager.hpp"


#include "sbe/util/NamedList.hpp"
#include "sbe/io/IO.hpp"

#include "sbe/io/IOPlugin.hpp"
#include "io/plugins/ImageSetIOPlugin.hpp"
#include "io/plugins/SoundIOPlugin.hpp"
#include "io/plugins/ImageIOPlugin.hpp"
#include "io/plugins/ShaderIOPlugin.hpp"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{

    DumpDebugInfo();
}

void ResourceManager::init()
{
	mIO = Engine::GetIO();

	loadDefaultPlugins();

	Engine::out(Engine::INFO) << "[ResourceManager] Adding default font" << std::endl;
    sf::Font* f = new sf::Font(getDefaultFont());
    add( f, "default");
}

bool ResourceManager::isResource( const std::type_index& ti )
{
    if ( Resources.find(ti) == Resources.end() )
    {
        Engine::out(Engine::ERROR) << "[ResourceManager] Class " << ti.name() << " not registered as Resource!" << std::endl;
        return false;
    }

    return true;
}

void ResourceManager::loadDefaultPlugins()
{
	Engine::out(Engine::INFO) << "[ResourceManager] IO plugins" << std::endl;

	std::shared_ptr<IOPlugin> SoundIOP ( new SoundIOPlugin() );
	registerResource<sf::SoundBuffer>( iResource::createResInfo("SoundBuffer", false, false), SoundIOP);

	std::shared_ptr<IOPlugin> ImgIOP ( new ImageIOPlugin() );
	registerResource<sf::Image>( iResource::createResInfo("Image", false, false), ImgIOP);

	std::shared_ptr<IOPlugin> ShaderIOP ( new ShaderIOPlugin() );
	registerResource<sf::Shader>( iResource::createResInfo("SHADER", false, false), ShaderIOP);

	std::shared_ptr<IOPlugin> ImageSetIOP ( new ImageSetIOPlugin() );
	registerResource<ImageSet>( iResource::createResInfo("ImageSet", false, false), ImageSetIOP);



	registerResource<sf::Font>( "Font" );

	//Engine::out() << "[ResourceManager] IO Plugins loaded." << std::endl;
}


void ResourceManager::DumpDebugInfo()
{

    Engine::out(Engine::INFO) << "[ResourceManager] Resources Overview:" << std::endl;
    for (auto &it : Resources)
    {
        Engine::out(Engine::INFO) << "Class: " << ResInfos[it.first].name << std::endl;

        (it.second)->DebugDump();
    }

    Engine::out(Engine::INFO) << "[ResourceManager] done." << std::endl;
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
