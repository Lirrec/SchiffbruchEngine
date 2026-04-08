#ifndef SFMLHELPERS_HPP
#define SFMLHELPERS_HPP

/**
	@file "SfmlHelpers.hpp" Helper Functions regarding conversion and sfml Rectangles/points/vectors
*/

#include <sbe/geom/Point.hpp>
#include <sbe/geom/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace sbe
{
	namespace geom
	{
		/// makes sure the given position is inside the given rectangle
		inline sf::Vector2f clip(const sf::Vector2f vec, const sf::FloatRect& limits) {
			sf::Vector2f result = vec;
			if (result.x < limits.position.x) result.x = limits.position.x;
			else if (result.x > limits.position.x + limits.size.x) result.x = limits.position.x + limits.size.x;

			if (result.y < limits.position.y) result.y = limits.position.y;
			else if (result.y > limits.position.y + limits.size.y) result.y = limits.position.y + limits.size.y;

			return result;
		}

		/// Construction helper for sf rectangles ( takes 2 points instead of 4 floats )
		inline sf::FloatRect makeSfRectf(const sf::Vector2f& topleft, const sf::Vector2f& bottomright) {
			return sf::FloatRect(topleft, {bottomright.x - topleft.x, bottomright.y - topleft.y});
		}

		/// Construction helper for sf rectangles ( takes 2 points instead of 4 ints )
		inline sf::IntRect makeSfRect(const sf::Vector2i& topleft, const sf::Vector2i& bottomright) {
			return sf::IntRect(topleft, {bottomright.x - topleft.x, bottomright.y - topleft.y});
		}

		/// converts a geom::irect to sf::FloatRect
		inline sf::FloatRect toSFFloatRect(const irect& r) {
			return sf::FloatRect({static_cast<float>(r.x.x), static_cast<float>(r.x.y)},
			                     {static_cast<float>(r.y.x - r.x.x), static_cast<float>(r.y.y - r.x.y)});
		}

		/// converts a geom::irect to sf::IntRect
		inline sf::IntRect toSFRect(const irect& r) {
			return sf::IntRect({r.x.x, r.x.y}, {r.y.x - r.x.x, r.y.y - r.x.y});
		}

		/// converts a sf::FloatRect to sf::IntRect ( ! Loss of fp precision ! )
		inline sf::IntRect SFRectFloatToInt(const sf::FloatRect& r) {
			return sf::IntRect(
				{static_cast<int>(r.position.x), static_cast<int>(r.position.y)},
				{static_cast<int>(r.size.x),     static_cast<int>(r.size.y)}
			);
		}

		/// converts a sf::IntRect to geom::irect
		inline irect fromSFRect(const sf::IntRect& r) {
			return irect{glm::ipoint2(r.position.x, r.position.y),
			             glm::ipoint2(r.position.x + r.size.x, r.position.y + r.size.y)};
		}

		/// converts a sf::FloatRect to geom::irect ( ! Loss of fp precision ! )
		inline irect fromSFFloatRect(const sf::FloatRect& r) {
			return fromSFRect(SFRectFloatToInt(r));
		}
	} // namespace geom
} // namespace sbe

#endif // SFMLHELPERS_HPP
