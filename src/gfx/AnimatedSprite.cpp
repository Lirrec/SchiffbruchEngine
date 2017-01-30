#include <sbe/gfx/AnimatedSprite.hpp>

#include <sbe/ResourceManager.hpp>

#include <sbe/geom/SfmlHelpers.hpp>
#include <sbe/geom/PointHelpers.hpp>

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
		auto pos = ScreenPosition + geom::PIToF(AnimData->DestPos);
		s.setPosition( {pos.x, pos.y} );
	}

	sf::Sprite& AnimatedSprite::getSprite() {
		return s;
	}

	AnimatedSprite::~AnimatedSprite() {

	}

	void AnimatedSprite::setScreenPosition(glm::point2 p) {
		Animation::setScreenPosition(p);
		updateDrawable();
	}
} // namespace sbe
