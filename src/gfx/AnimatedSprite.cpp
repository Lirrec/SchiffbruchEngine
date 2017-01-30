#include "sbe/gfx/AnimatedSprite.hpp"

#include "sbe/ResourceManager.hpp"

#include "sbe/geom/SfmlHelpers.hpp"

namespace sbe
{
	AnimatedSprite::AnimatedSprite(const std::shared_ptr<ImageSet>& _A)
			: Animation(_A) {
		std::shared_ptr<sf::Texture> txt = AnimData->getTexture();
		if (txt)
		{
			s.setTexture(*txt);
		}
		else
		{
			Engine::out() << "AnimatedSprite unable to find texture " << AnimData->ImageName << "!" << std::endl;
		}
	}

	void AnimatedSprite::setImageSet(const std::shared_ptr<ImageSet>& _A) {
		Animation::setImageSet(_A);

		std::shared_ptr<sf::Texture> txt = AnimData->getTexture();
		if (txt)
		{
			s.setTexture(*txt);
			updateDrawable();
		}
		else
		{
			Engine::out() << "AnimatedSprite unable to find texture " << AnimData->ImageName << "!" << std::endl;
		}

	}

	void AnimatedSprite::updateDrawable() {
		s.setTextureRect(geom::toSFRect(AnimData->CalcTexCoords(CurFramePos)));
		s.setPosition((float) Screen_Position.x, (float) Screen_Position.y);
	}

	sf::Sprite& AnimatedSprite::getSprite() {
		return s;
	}

	AnimatedSprite::~AnimatedSprite() {

	}
} // namespace sbe
