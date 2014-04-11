#include "sbe/gfx/Actor.hpp"

#include "sbe/Module.hpp"

#include <boost/uuid/uuid_generators.hpp>
namespace sbe
{
	Actor::Actor()
	 : ID( boost::uuids::random_generator()() )
	{
	}

	sf::Drawable& SpriteActor::getDrawable()
	{
		return sprite;
	}

	sf::Drawable& VertexActor::getDrawable()
	{
		return arr;
	}

	sf::Drawable& DrawableActor::getDrawable()
	{
		return *d;
	}

	AnimationActor::AnimationActor( ImageSet& ImgSet)
	 : Anim( ImgSet )
	{
	}

	void AnimationActor::update( const sf::Time& RenderTime )
	{
		Anim.update ( RenderTime );
	}

	sf::Drawable& AnimationActor::getDrawable()
	{
		return Anim.getSprite();
	}

	sf::Drawable& ShapeActor::getDrawable()
	{
		return *Shape;
	}
} // namespace sbe
