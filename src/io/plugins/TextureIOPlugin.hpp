#ifndef TEXTUREIOPLUGIN_HPP
#define TEXTUREIOPLUGIN_HPP

#include <SFML/Graphics/Texture.hpp>

#include <istream>
#include <ostream>

#include "sbe/io/IOPlugin.hpp"

class TextureIOPlugin : iBinaryIOPlugin<sf::Texture> {

	TextureIOPlugin();

	virtual ~TextureIOPlugin();

	virtual ObjectList decodeStream(std::istream &in);
	virtual bool encodeStream( const sf::Texture &o, std::ostream &out);
	virtual std::vector<std::string> getSupportedFileExtensions();
};

#endif // TERRAINIOPLUGIN_HPP

