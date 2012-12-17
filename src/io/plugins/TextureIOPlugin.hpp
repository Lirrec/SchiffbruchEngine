#ifndef TEXTUREIOPLUGIN_HPP
#define TEXTUREIOPLUGIN_HPP

#include <SFML/Graphics/Texture.hpp>

#include <istream>
#include <ostream>

#include "sbe/io/IOPlugin.hpp"

class TextureIOPlugin : iBinaryIOPlugin<sf::Texture> {

	TextureIOPlugin();

	virtual ~TextureIOPlugin();

	virtual ObjectList decodeStream(std::istream &in) override;
	virtual bool encodeStream( const Texture &o, std::ostream &out) override;
	virtual std::vector<std::string> getSupportedFileExtensions() override;
};

#endif // TERRAINIOPLUGIN_HPP

