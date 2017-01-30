#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <boost/uuid/uuid.hpp>
#include <sbe/geom/Rect.hpp>

namespace sf {
	class Drawable;
	class Time;
}

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

} // namespace sbe
#endif // GRAPHIC_H
