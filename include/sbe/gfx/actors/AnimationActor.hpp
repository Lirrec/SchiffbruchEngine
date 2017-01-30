#ifndef SCHIFFBRUCHENGINE_ANIMATIONACTOR_HPP
#define SCHIFFBRUCHENGINE_ANIMATIONACTOR_HPP

#include <sbe/gfx/Actor.hpp>

#include <sbe/gfx/AnimatedSprite.hpp>

namespace sbe
{

	/**
		An Actor holding an AnimatedSprite
	*/
	class AnimationActor : public Actor
	{
	public:
		AnimationActor(const std::shared_ptr<ImageSet>& ImgSet);

		void update(const sf::Time& RenderTime) override;

		sf::Drawable& getDrawable()  override;
		geom::rect getBounds() override;

		AnimatedSprite Anim;

		void setAnimation(const std::string& name);
	};

} // namespace sbe
#endif //SCHIFFBRUCHENGINE_ANIMATIONACTOR_HPP
