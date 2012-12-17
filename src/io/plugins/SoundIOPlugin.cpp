#include "SoundIOPlugin.hpp"

SoundIOPlugin::SoundIOPlugin()
{
    path = "sounds";
}

SoundIOPlugin::~SoundIOPlugin()
{

}

ObjectList SoundIOPlugin::decodeStream(std::istream& in)
{
	ObjectList re;
    sf::SoundBuffer SB;
    if ( SB.loadFromStream( sfIStream( in ) ) )
    {
		re.push_back( SB );
    }

    return re;
}

bool SoundIOPlugin::encodeStream(const sf::SoundBuffer& o, std::ostream& out)
{
	Engine::out() << "[SoundIOPlugin] Sorry, saving not implemented!" << std::endl;
	return false;
}

std::vector<std::string> SoundIOPlugin::getSupportedFileExtensions()
{
    static std::vector<std::string> fileexts { "ogg", "wav", "flac", "aiff", "au" };
    return fileexts;
}
