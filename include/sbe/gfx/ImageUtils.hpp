#ifndef IMAGEUTIL_HPP
#define IMAGEUTIL_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "sbe/geom/Point.hpp"

namespace sbe
{
	namespace gfx
	{

		/**
			Set the alpha channel of an image
		*/
		inline void SetImageAlpha( sf::Image& img, int a )
		{
			if ( a < 0 || a > 255 ) a = 0;

			sf::Vector2u Size = img.getSize();

			for(int x = 0; x < Size.x; x++)
			{
				for(int y = 0; y < Size.y; y++)
				{
					sf::Color C = img.getPixel(x,y);
					C.a = a;
					img.setPixel(x,y, C);
				}
			}
		}

		/**
			Uses gpu scaling by rendering the image to a larger rendertexture.
			@param the Image to scale
			@param newSize the target size
		*/
		inline sf::Image ScaleImage( sf::Image& img, glm::ivec2 newSize )
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


} // namespace sbe
#endif // IMAGEUTIL_HPP

