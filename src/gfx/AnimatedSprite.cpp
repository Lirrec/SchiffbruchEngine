#include "sbe/gfx/AnimatedSprite.hpp"

#include "sbe/ResourceManager.hpp"

#include "sbe/geom/Helpers.hpp"

namespace sbe
{
	AnimatedSprite::AnimatedSprite( ImageSet& _A )
		: Animation ( _A )
	{
		std::shared_ptr<sf::Texture> txt = Engine::GetResMgr()->get<sf::Texture>(AnimData.ImageName);
		if (txt) {
			s.setTexture(*txt);
		}
	}

	void AnimatedSprite::updateDrawable()
	{
		s.setTextureRect( Geom::toSFRect( AnimData.CalcTexCoords(CurFramePos) ) );
		s.setPosition( (float)Screen_Position.x, (float)Screen_Position.y );
	}

	sf::Sprite& AnimatedSprite::getSprite()
	{
		return s;
	}
} // namespace sbe
