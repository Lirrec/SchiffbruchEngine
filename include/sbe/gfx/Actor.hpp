#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <boost/uuid/uuid.hpp>

#include "sbe/gfx/AnimatedSprite.hpp"

#include <string>
#include <memory>

#include <boost/uuid/uuid.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sbe
{
	/// unique ID of an Actor
	typedef boost::uuids::uuid ActorID;

	/**
		An Actor defines an entity which represents a graphical element and is rendered by the Renderer.
		@see Renderer
	*/
	class Actor
	{
	public:

		Actor();

		virtual ~Actor() { }

		virtual void update(const sf::Time& RenderTime) { };

		virtual sf::Drawable& getDrawable() = 0;

		/**
		 * Should return a bounding for for the Actor.
		 * If the Actor is unable to provide a boundingbox an empty rect ( 0,0,0,0 ) should be returned
		 * @return
		 */
		virtual geom::rect getBounds() =0;

		ActorID getID() const { return ID; }

		bool enabled = true;

	private:
		const ActorID ID;
	};

	/**
		An Actor holding a simple sf::Sprite
	*/
	class SpriteActor : public Actor
	{
	public:
		sf::Drawable& getDrawable()  override;
		geom::rect getBounds() override;

		sf::Sprite sprite;

	};

	/**
		An Actor holding a sf::VertexArray
	*/
	class VertexActor : public Actor
	{
	public:
		sf::Drawable& getDrawable()  override;
		geom::rect getBounds() override;

		sf::VertexArray arr;
	};

	/**
		An Actor holding a sf::VertexArray
	*/
	class DrawableActor : public Actor
	{
	public:
		sf::Drawable& getDrawable()  override;
		geom::rect getBounds() override;

		std::shared_ptr<sf::Drawable> d;
	};

	/**
		An Actor holding an AnimatedSprite
	*/
	class AnimationActor : public Actor
	{
	public:
		AnimationActor(ImageSet& ImgSet);

		void update(const sf::Time& RenderTime) override;

		sf::Drawable& getDrawable()  override;
		geom::rect getBounds() override;

		AnimatedSprite Anim;
	};

	/**
		An Actor holding an AnimatedSprite
	*/
	class ShapeActor : public Actor
	{
	public:
		sf::Drawable& getDrawable()  override;
		geom::rect getBounds() override;

		std::shared_ptr<sf::Shape> Shape;
	};

	//class VertexAnimationActor : public Actor {
	//	public:
	//		void draw( sf::RenderTarget& t) { t.draw ( arr ); }
	//		void update( const sf::Time& RenderTime ) {  }
	//	private:
	//		Animation Anim;
	//		sf::VertexArray arr;
	//};

} // namespace sbe
#endif // GRAPHIC_H
