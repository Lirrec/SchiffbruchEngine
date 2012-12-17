#ifndef SOUNDIOPLUGIN_HPP
#define SOUNDIOPLUGIN_HPP

#include "sbe/ImageSet.hpp"
#include "sbe/io/IOPlugin.hpp"

#include <SFML/Audio/SoundBuffer.hpp>

#include <istream>
#include <ostream>

class SoundIOPlugin : public iBinaryIOPlugin<sf::SoundBuffer> {

	SoundIOPlugin();

	virtual ~SoundIOPlugin();

	virtual ObjectList decodeStream(std::istream &in) override;
	virtual bool encodeStream( const sf::SoundBuffer &o, std::ostream &out) override;
	virtual std::vector<std::string> getSupportedFileExtensions() override;
};

#endif // SOUNDIOPLUGIN_HPP
