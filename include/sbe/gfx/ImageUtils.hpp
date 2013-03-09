#ifndef IMAGEUTIL_HPP
#define IMAGEUTIL_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "sbe/geom/Point.hpp"

namespace gfx
{

	/**
		Uses gpu scaling by rendering the image to a larger rendertexture.
		@param the Image to scale
		@param newSize the target size
	*/
	sf::Image ScaleImage( sf::Image& img, Geom::Vec2 newSize )
	{
		/// scale the image by rendering it bigger onto a rendertexture, not nice
		sf::Texture tex;
		tex.loadFromImage( img );

		sf::Sprite spriteTmp( tex);
		spriteTmp.scale(newSize.x/img.getSize().x, newSize.y/img.getSize().y );

		sf::RenderTexture target;
		target.create( newSize.x, newSize.y );
		target.clear(sf::Color(0,0,0,255));
		target.draw(spriteTmp);
		target.display();

		return target.getTexture().copyToImage();
	}


}



#endif // IMAGEUTIL_HPP

