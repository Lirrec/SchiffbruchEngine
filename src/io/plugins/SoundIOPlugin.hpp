#ifndef SOUNDIOPLUGIN_HPP
#define SOUNDIOPLUGIN_HPP

#include "sbe/gfx/ImageSet.hpp"
#include "sbe/io/IOPlugin.hpp"

#include <SFML/Audio/SoundBuffer.hpp>

#include <istream>
#include <ostream>

/// iBinaryIOPluing for sf::SoundBuffer
class SoundIOPlugin : public iBinaryIOPlugin<sf::SoundBuffer>
{
public:

    SoundIOPlugin();

    virtual ~SoundIOPlugin();

    virtual ObjectList decodeStream(std::istream &in);
    virtual bool encodeStream( const sf::SoundBuffer &o, std::ostream &out);
    virtual const std::vector<std::string>& getSupportedFileExtensions();
};

#endif // SOUNDIOPLUGIN_HPP
