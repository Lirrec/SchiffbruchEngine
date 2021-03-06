#ifndef RECT_HPP
#define RECT_HPP

#include <glm/vec2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <cmath>

namespace sbe
{
	/**
		This namespace holds various Geometry and Math related functions and classes
	*/
	namespace geom
	{
		/// a rectangle. Consists of 2 points, top-left and bottom-right position of the rect
		class rect
		{
		public:
			rect() = default;

			rect(sf::FloatRect fr) : x({fr.left, fr.top}),
			                                            // plus v here because our origin is top left, "bottom" is a higher y value
			                         y({fr.left+fr.width, fr.top+fr.height})
			{}

			operator sf::FloatRect() { return { x.x, x.y, std::fabs(x.x - y.x), std::fabs(x.y -y.y) }; }
			rect operator/ (int i) const { return {{x.x/i, x.y/i},{y.x/i, y.y/i}};}
			rect operator* (int i) const { return {{x.x*i, x.y*i},{y.x*i, y.y*i}};}
			rect operator+ (int i) const { return {{x.x+i, x.y+i},{y.x+i, y.y+i}};}
			rect operator- (int i) const { return {{x.x-i, x.y-i},{y.x-i, y.y-i}};}

			rect(const glm::vec2& _x, const glm::vec2& _y) : x(_x), y(_y) { }

			glm::vec2 x;
			glm::vec2 y;
		};

		/// an integer rectangle. Consists of 2 points, top-left and bottom-right position of the rect
		class irect
		{
		public:
			irect() = default;
			irect(sf::IntRect ir) : x({ir.left, ir.top}),
														// plus v here because our origin is top left, "bottom" is a higher y value
									y({ir.left+ir.width, ir.top+ir.height})
			{}

			operator sf::IntRect() { return { x.x, x.y, std::abs(x.x - y.x), std::abs(x.y -y.y) }; }

			irect operator/ (int i) const { return {{x.x/i, x.y/i},{y.x/i, y.y/i}};}
			irect operator* (int i) const { return {{x.x*i, x.y*i},{y.x*i, y.y*i}};}
			irect operator+ (int i) const { return {{x.x+i, x.y+i},{y.x+i, y.y+i}};}
			irect operator- (int i) const { return {{x.x-i, x.y-i},{y.x-i, y.y-i}};}


			irect(const glm::ivec2& _x, const glm::ivec2& _y) : x(_x), y(_y) { }

			glm::ivec2 x;
			glm::ivec2 y;
		};
	}
} // namespace sbe

namespace geom = sbe::geom;

#endif // RECT_HPP
