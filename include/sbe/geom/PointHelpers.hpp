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
	} // namespace Geom
} // namespace sbe

#endif // POINTHELPERS_HPP

