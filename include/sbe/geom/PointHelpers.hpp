#ifndef ipointHELPERS_HPP
#define ipointHELPERS_HPP

#include "sbe/geom/point.hpp"


/**
	@file "Helpers.hpp" Functions regarding ipoints and Vectors
*/

namespace sbe
{
	namespace geom
	{
		/// returns the origin as integer point (0,0)
		inline glm::ipoint2 iOrigin() { return glm::ipoint2(0,0); }
		/// returns the origin (0.0,0.0)
		inline glm::point2 Origin() { return glm::point2(0.0f,0.0f); }

		/// convert a ipoint to point(float),
		inline glm::point2 PIToF( const glm::ipoint2& p )
		{
			return glm::point2( p.x, p.y );
		}

		/// convert a point(float) to ipoint(int), precision loss!
		inline glm::ipoint2 PFToI( const glm::point2& p )
		{
			return glm::ipoint2( (int)p.x, (int)p.y );
		}
	} // namespace geom
} // namespace sbe

#endif // ipointHELPERS_HPP

