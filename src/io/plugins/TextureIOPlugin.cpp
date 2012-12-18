#include "TextureIOPlugin.hpp"

#include "sbe/io/SFistream.hpp"

#include "sbe/Engine.hpp"

TextureIOPlugin::TextureIOPlugin()
{
    path = "textures";
}

TextureIOPlugin::~TextureIOPlugin()
{

}

TextureIOPlugin::ObjectList TextureIOPlugin::decodeStream(std::istream& in)
{
	ObjectList re;
    std::shared_ptr<sf::Texture> txt ( new sf::Texture() );
    sfIStream stream( in );
    if ( txt->loadFromStream( stream ) )
    {
		re.push_back( txt );
    }

    return re;
}

bool TextureIOPlugin::encodeStream(const sf::Texture& o, std::ostream& out)
{
	Engine::out() << "[TextureIOPlugin] Sorry, saving not implemented!" << std::endl;
	return false;
}

std::vector<std::string> TextureIOPlugin::getSupportedFileExtensions()
{
    static std::vector<std::string> fileexts { "png", "jpeg", "jpg", "bmp"  };
    return fileexts;
}

