#ifndef RECT_HPP
#define RECT_HPP

#include <glm/vec2.hpp>

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

			rect(const glm::vec2& _x, const glm::vec2& _y) : x(_x), y(_y) { }

			glm::vec2 x;
			glm::vec2 y;
		};

		/// an integer rectangle. Consists of 2 points, top-left and bottom-right position of the rect
		class irect
		{
		public:
			irect() = default;

			irect(const glm::ivec2& _x, const glm::ivec2& _y) : x(_x), y(_y) { }

			glm::ivec2 x;
			glm::ivec2 y;
		};
	}
} // namespace sbe

namespace geom = sbe::geom;

#endif // RECT_HPP
