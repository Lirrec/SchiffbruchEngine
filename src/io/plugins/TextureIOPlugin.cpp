#include "TextureIOPlugin.hpp"

TextureIOPlugin::TextureIOPlugin()
{
    path = "textures";
}

TextureIOPlugin::~TextureIOPlugin()
{

}

ObjectList TextureIOPlugin::decodeStream(std::istream& in)
{
	ObjectList re;
    sf::Texture txt;
    if ( txt.loadFromStream( sfIStream( in ) ) )
    {
		re.push_back( txt );
    }

    return re;
}

bool TextureIOPlugin::encodeStream(const Texture& o, std::ostream& out)
{
	Engine::out() << "[TextureIOPlugin] Sorry, saving not implemented!" << std::endl;
	return false;
}

std::vector TextureIOPlugin::getSupportedFileExtensions()
{
    static std::vector<std::string> fileexts { "png", "jpeg", "jpg", "bmp"  };
    return fileexts;
}

