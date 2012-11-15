#ifndef TEXTUREIOPLUGIN_HPP
#define TEXTUREIOPLUGIN_HPP

#include <SFML/Graphics/Texture.hpp>

#include "sbe/io/IOPlugin.hpp"

class TextureIOPlugin : iBinaryIOPlugin<sf::Texture> {

	TextureIOPlugin();

	virtual ~TextureIOPlugin();

	ObjectList decodeStream(std::istream &in) override;
	bool encodeStream( const Texture &o, std::ostream &out) override;
	std::vector<std::string> getSupportedFileExtensions() override;
};

#endif // TERRAINIOPLUGIN_HPP

