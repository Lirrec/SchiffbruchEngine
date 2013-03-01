#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <boost/uuid/uuid.hpp>

#include "sbe/Geom.hpp"
#include "sbe/gfx/AnimatedSprite.hpp"

#include <string>
#include <memory>

#include <boost/uuid/uuid.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

typedef boost::uuids::uuid ActorID;

class Actor {
	public:

	Actor();
	virtual ~Actor() {}

	virtual void update( const sf::Time& RenderTime ) {};
	virtual sf::Drawable& getDrawable() = 0;
	ActorID getID() { return ID; }

	private:
		const ActorID ID;
};

class SpriteActor : public Actor
{
	public:
		sf::Drawable& getDrawable();
		sf::Sprite sprite;

};

class VertexActor : public Actor
{
	public:
		sf::Drawable& getDrawable();
		sf::VertexArray arr;
};

class AnimationActor : public Actor {
	public:
		void update( const sf::Time& RenderTime );
		sf::Drawable& getDrawable();
		AnimatedSprite Anim;
};

//class VertexAnimationActor : public Actor {
//	public:
//		void draw( sf::RenderTarget& t) { t.draw ( arr ); }
//		void update( const sf::Time& RenderTime ) {  }
//	private:
//		Animation Anim;
//		sf::VertexArray arr;
//};

#endif // GRAPHIC_H
