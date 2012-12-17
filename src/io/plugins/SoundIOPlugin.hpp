#ifndef SOUNDIOPLUGIN_HPP
#define SOUNDIOPLUGIN_HPP

#include "sbe/ImageSet.hpp"

#include <sf::SoundBuffer>

#include <istream>
#include <ostream>

class SoundIOPlugin : iBinaryIOPlugin<sf::SoundBuffer> {

	SoundIOPlugin();

	virtual ~ISoundIOPlugin();

	virtual ObjectList decodeStream(std::istream &in) override;
	virtual bool encodeStream( const sf::SoundBuffer &o, std::ostream &out) override;
	virtual std::vector<std::string> getSupportedFileExtensions() override;
};

#endif // SOUNDIOPLUGIN_HPP
