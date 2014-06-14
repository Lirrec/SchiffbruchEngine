#ifndef RECT_HPP
#define RECT_HPP

#include <glm/vec2.hpp>

namespace sbe
{
	/**
		This namespace holds various Geometry and Math related functions and classes
	*/
	namespace Geom
	{
		/// a rectangle. Consists of 2 points, top-left and bottom-right position of the rect
		struct rect {
			glm::vec2 x;
			glm::vec2 y;
		};
		/// an integer rectangle. Consists of 2 points, top-left and bottom-right position of the rect
		struct irect {
			glm::ivec2 x;
			glm::ivec2 y;
		};
	}
} // namespace sbe

namespace Geom = sbe::Geom;

#endif // RECT_HPP
