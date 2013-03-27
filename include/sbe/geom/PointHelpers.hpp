#ifndef POINTHELPERS_HPP
#define POINTHELPERS_HPP

#include "sbe/geom/Point.hpp"

/**
	@file "Helpers.hpp" Functions regarding Points and Vectors
*/

namespace sbe
{
	namespace Geom
	{

		/// returns the origin (0,0)
		inline Point Origin() { return Point(0,0); }
		/// returns the origin (0.0,0.0)
		inline Pointf Originf() { return Pointf(0.0f,0.0f); }

		/// convert a point<int> to point<float>,
		inline Pointf PIToF( const Point& p )
		{
			return Pointf( p.x, p.y );
		}

		/// convert a point<float> to point<int>, precision loss!
		inline Point PFToI( const Pointf& p )
		{
			return Point( (int)p.x, (int)p.y );
		}


		/// calculates the squareddistance between two points (avoiding the sqrt, but still fine for comparisions)
		inline float squaredist(const Point& from, const Point& to)
		{
			return std::abs(std::pow(from.x - to.x, 2)
					+ std::pow(from.y - to.y, 2));
		}

		inline float squaredist(const Pointf& from, const Pointf& to)
		{
			return std::abs(std::pow(from.x - to.x, 2) + std::pow(from.y - to.y, 2));
		}

		/// calculates the distance between two points (just simple and not optimized)
		inline float distance(const Point& from, const Point& to)
		{
			return std::sqrt( squaredist(from,to) );
		}

		/// Compute the distance between two points ( floating point version )
		inline float distance(const Pointf& from, const Pointf& to)
		{
			return std::sqrt( squaredist( from,to ) );
		}

		/// Compute the length of a vector
		inline float length( Vec2f V )
		{
				return distance( Originf(), V );
		}

		/// Normalize a vector
		inline Vec2f normalize( Vec2f V )
		{
				return V / Vec2f(length(V), length(V));
		}

	} // namespace Geom
} // namespace sbe

#endif // POINTHELPERS_HPP

