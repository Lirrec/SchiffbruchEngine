#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP

#include "sbe/gfx/Animation.hpp"
#include <SFML/Graphics/Sprite.hpp>

namespace sbe
{
	/**

		An Animated sf::Sprite.
	*/
	class AnimatedSprite : public Animation
	{
	public:

		AnimatedSprite(const std::shared_ptr<ImageSet>& Anim);

		~AnimatedSprite() override;

		/// update the associated sprite/vertexarray to show the correct sub-rect/frame of the image
		void updateDrawable() override;

		void setImageSet(const std::shared_ptr<ImageSet>& _A) override;

		/// return the managed Sprite
		sf::Sprite& getSprite();

	private:
		sf::Sprite s;

	};
} // namespace sbe
#endif // ANIMATEDSPRITE_HPP


