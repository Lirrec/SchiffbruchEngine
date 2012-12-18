#include "ImageIOPlugin.hpp"

#include "sbe/io/SFistream.hpp"

#include "sbe/Engine.hpp"

ImageIOPlugin::ImageIOPlugin()
{
    path = "textures";
}

ImageIOPlugin::~ImageIOPlugin()
{

}

ImageIOPlugin::ObjectList ImageIOPlugin::decodeStream(std::istream& in)
{
	ObjectList re;
    std::shared_ptr<sf::Image> txt ( new sf::Image() );
    sfIStream stream( in );
    if ( txt->loadFromStream( stream ) )
    {
		re.push_back( txt );
    }

    return re;
}

bool ImageIOPlugin::encodeStream(const sf::Image& o, std::ostream& out)
{
	Engine::out() << "[ImageIOPlugin] Sorry, saving not implemented!" << std::endl;
	return false;
}

std::vector<std::string> ImageIOPlugin::getSupportedFileExtensions()
{
    static std::vector<std::string> fileexts { "png", "jpeg", "jpg", "bmp"  };
    return fileexts;
}
