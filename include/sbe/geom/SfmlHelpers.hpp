#ifndef SFMLHELPERS_HPP
#define SFMLHELPERS_HPP

/**
	@file "SfmlHelpers.hpp" Helper Functions regarding conversion and sfml Rectangles/points/vectors
*/

#include "sbe/geom/Point.hpp"
#include <SFMl/System/Vector2.hpp>
#include <SFMl/Graphics/Rect.hpp>

namespace sbe
{
	namespace Geom
	{
		/// makes sure the given position is inside the given rectangle
		inline sf::Vector2f clip( const sf::Vector2f vec, const sf::FloatRect& limits )
		{
				sf::Vector2f result = vec;
				if ( result.x < limits.left ) result.x = limits.left;
				else if ( result.x > limits.left + limits.width ) result.x = limits.left + limits.width;

				if ( result.y < limits.top ) result.y = limits.top;
				else if ( result.y > limits.top + limits.height ) result.y = limits.top + limits.height;

				return result;
		}

		/// Construction helper for sf rectangles ( takes 2 points instead of 4 floats )
		inline sf::FloatRect makeSfRectf ( const sf::Vector2f& topleft, const sf::Vector2f& bottomright)
		{
			return sf::FloatRect( topleft.x, topleft.y, bottomright.x - topleft.x, bottomright.y - topleft.y );
		}

		/// Construction helper for sf rectangles ( takes 2 points instead of 4 ints )
		inline sf::IntRect makeSfRect ( const sf::Vector2i& topleft, const sf::Vector2i& bottomright)
		{
			return sf::IntRect( topleft.x, topleft.y, bottomright.x - topleft.x, bottomright.y - topleft.y );
		}

				/// converts a Geom::Rect to sf::Floatrect
		inline sf::FloatRect toSFFloatRect ( const Rect& r)
		{
			return sf::FloatRect( r.x.x, // left
								r.x.y, // top
								r.y.x-r.x.x, // width
								r.y.y-r.x.y ); // height
		}

		/// converts a Geom::Rect to sf::IntRect
		inline sf::IntRect toSFRect(const Rect& r)
		{
			return sf::IntRect( r.x.x, // left
								r.x.y, // top
								r.y.x-r.x.x, // width
								r.y.y-r.x.y ); // height
		}

		/// converts a sf::FloatRect to sf::IntRect ( ! Loss of fp precision ! )
		inline sf::IntRect SFRectFloatToInt(const sf::FloatRect& r)
		{
			sf::IntRect re ( static_cast<int>(r.left),
							 static_cast<int>(r.top),
							 static_cast<int>(r.width),
							 static_cast<int>(r.height)
							);
			return re;
		}

		/// converts a sf::IntRect to Geom::Rect
		inline Rect fromSFRect(const sf::IntRect& r)
		{
			return Rect(Point(r.left, r.top), Point(r.left + r.width, r.top + r.height));
		}

		/// converts a sf::FloatRect to Geom::Rect ( ! Loss of fp precision ! )
		inline Rect fromSFFloatRect ( const sf::FloatRect& r )
		{
			return fromSFRect( SFRectFloatToInt(r) );
		}
	} // namespace Geom
} // namespace sbe

#endif // SFMLHELPERS_HPP

