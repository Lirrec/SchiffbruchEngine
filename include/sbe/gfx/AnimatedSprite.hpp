#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP

#include "sbe/gfx/Animation.hpp"
#include <SFML/Graphics/Sprite.hpp>


class AnimatedSprite : public Animation
{
public:

		AnimatedSprite( ImageSet& _A );

		/// update the associated sprite/vertexarray to show the correct sub-rect/frame of the image
		void updateDrawable();
		sf::Sprite& getSprite();
	private:
		sf::Sprite s;

};

#endif // ANIMATEDSPRITE_HPP


