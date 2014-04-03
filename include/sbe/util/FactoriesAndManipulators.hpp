#ifndef FACTORIESANDMANIPULATORS_HPP
#define FACTORIESANDMANIPULATORS_HPP

/**
	@File This File contains helper functions for creating objects/types an manipulations them.
	This includes factories to create objects with given limits ( eg. a float in range 0-1 )
	and interpolators to interpolate between 2 floats, 2 points, etc.

	The random functions use the same RNG and are therefore not thread-safe!
*/

#include <sbe/geom/Point.hpp>
#include <SFML/Graphics/Color.hpp>
#include <random>

namespace sbe
{
	std::default_random_engine& getRNG();

	namespace factories {

			inline float randomInt(const Geom::Vec2 limits)
			{
				std::uniform_int_distribution<int> uniform_dist(limits.x, limits.y);
				return uniform_dist(getRNG());
			}

			inline float randomFloat(const Geom::Vec2f limits)
			{
				std::uniform_real_distribution<float> uniform_dist(limits.x, limits.y);
				return uniform_dist(getRNG());
			}

			inline Geom::Pointf randomPointf(const Geom::Rectf limits)
			{
				return { randomFloat( {limits.x.x, limits.y.x} ), randomFloat({limits.x.y, limits.y.y}) };
			}

			inline Geom::Point randomPoint(const Geom::Rect limits)
			{
				return { randomInt( {limits.x.x, limits.y.x} ), randomInt({limits.x.y, limits.y.y}) };
			}

			inline sf::Color randomColor()
			{
				std::uniform_int_distribution<int> uniform_dist(0,255);
				return sf::Color(uniform_dist(getRNG()), uniform_dist(getRNG()), uniform_dist(getRNG()));
			}
	}

	namespace interpolate {

			inline float interpolateInt(const int from, const int to, const float percentage)
			{
				return (int)(std::round(from + ( to - from ) * percentage));
			}

			inline float interpolateFloat(const float from, const float to, const float percentage)
			{
				return from + ( to - from ) * percentage;
			}

			sf::Color interpolateColor(const sf::Color from, const sf::Color to, const float percentage);
	}
}

#endif // FACTORIESANDMANIPULATORS_HPP

