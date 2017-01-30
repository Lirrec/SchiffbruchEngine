#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <boost/lexical_cast.hpp>

#include <SFML/Graphics/Rect.hpp>

#include <cmath>

#include <sbe/geom/Point.hpp>

/**
	@file "Helpers.hpp" General Helper functions
*/

namespace sbe
{
	namespace geom
	{

		/**
			Calculate the next power of 2 which is greater than x.
			old ref: http://acius2.blogspot.com/2007/11/calculating-next-power-of-2.html
		*/
		inline int nextpow2(int x) {
			int y = x;
			while (x &= (x ^ (~x + 1)))
				y = x << 1;
			return y;
		}

		/// returns the index of a 2d position in an array, assumes data is stored in rows, not columns
		inline size_t linear(int x, int y, int width) { return x + y*width; }

		/// returns the index of a 2d position in an array, assumes data is stored in rows, not columns
		inline size_t linear(glm::vec2 p, int width) { return linear(p.x, p.y, width); }

		/// returns the index of a 2d position in an array, assumes data is stored in rows, not columns
		inline size_t linear(glm::ivec2 p, int width) { return linear(p.x, p.y, width); }

		/**
			Normalize a float, i.e. scale it to a range from 0.0 to 1.0
			@param f the float to normalize
			@param min the minimum
			@param max the maximum
		*/
		inline float normalize(float f, float min, float max) {
			//assert ( max - min != 0 );
			if (max - min == 0) return 0;
			return (f - min)/(max - min);
		}

		/**
			Return the middle point of a line defined by p1 and p2
		*/
		inline glm::ipoint2 middle(const glm::ipoint2& p1, const glm::ipoint2& p2) {
			return (p1+p2)/2;
		}

		// TRIANGLE
		/**
			Checks wether a given point is inside a triangle
			@see PointInTri(glm::ipoint2 p, glm::ipoint2 t1, glm::ipoint2 t2, glm::ipoint2 t3) a version which takes points as parameters
		*/
		inline bool PointInTri(int X, int Y, int X0, int Y0, int X1, int Y1, int /*X3*/, int Y3) {
			float x, y,
					x0, y0,
					x1, y1,
			//x3,
					y3,
					a, b, c, d;

			x = (float) X;
			y = (float) Y;

			x0 = (float) X0;
			y0 = (float) Y0;

			x1 = (float) X1;
			y1 = (float) Y1;

			//x3=(float)X3;
			y3 = (float) Y3;

			c = (x - x1)/(x0 - x1);
			if (c < 0) return false;
			d = ((y - y3)*(x0 - x1) - (x - x1)*(y0 - y3))/((y1 - y3)*(x0 - x1));
			if (d < 0) return false;
			b = ((y - y0)*(x1 - x0) - (x - x0)*(y1 - y0))/((x1 - x0)*(y3 - y1));
			if (b < 0) return false;
			a = (x - x0)/(x1 - x0) - b;
			if (a < 0) return false;
			return true;
		}

		inline bool PointInTri(glm::ipoint2 p, glm::ipoint2 t1, glm::ipoint2 t2, glm::ipoint2 t3) {
			return PointInTri(p.x, p.y, t1.x, t1.y, t2.x, t2.y, t3.x, t3.y);
		}


	}    // namespace geom
} // namespace sbe
#endif // HELPERS_HPP
