#ifndef RECTHELPERS_HPP
#define RECTHELPERS_HPP

#include "sbe/geom/Point.hpp"


/**
	@file "Helpers.hpp" Functions regarding Rectangles
*/

namespace sbe
{
	namespace Geom
	{
		///Converts a rectangle to a human readable string suitable for debugging purposes
		inline std::string RectToString( const Geom::Rect& r)
		{
			std::string re("");
			re += "[";
			re += boost::lexical_cast<std::string>(r.x.x);
			re += "/";
			re += boost::lexical_cast<std::string>(r.x.y);
			re += "|";
			re += boost::lexical_cast<std::string>(r.y.x);
			re += "/";
			re += boost::lexical_cast<std::string>(r.y.y);
			re += "]";

			return re;
		}

			/// Checks wether p is inside rc
		inline bool PointInRect(const Rect& rc, const Point& p)
		{
				if (
					(p.x >= rc.x.x && p.x <= rc.y.x)
					&&
					(p.y >= rc.x.y && p.y <= rc.y.y)
					)
					return true;
			return false;
		}


		/// returns the width of a rectangle
		inline int rcWidth(const Rect& rc)
		{
			return std::abs(rc.y.x - rc.x.x);
		}

		/// returns the height of a rectangle
		inline int rcHeight(const Rect& rc)
		{
			return std::abs(rc.y.y - rc.x.y);
		}

		/// returns the top right point of a rectangle
		inline Point rcBLPoint(const Rect& rc)
		{
			return Geom::Point(rc.x.x, rc.y.y);
		}

		/// returns the top right point of a rectangle
		inline Point rcTRPoint(const Rect& rc)
		{
			return Geom::Point(rc.y.x , rc.x.y );
		}


		/// Construction helper for rectangles ( takes 4 ints instead of 2 points
		inline Rect makeRect(const int left, const int top, const int right, const int bottom)
		{
			return Rect( Point(left, top), Point(right, bottom) );
		}
		/// Construction helper for rectangles ( takes 4 floats instead of 2 pointfs
		inline Rectf makeRectf(const float left, const float top, const float right, const float bottom)
		{
			return Rectf( Pointf(left, top), Pointf(right, bottom) );
		}

		/// returns the 4 points of a rectangle ( clockwise starting at top-left )
		template< class T>
		inline void decompose( point<T>* re, const point<point<T>>& r )
		{
			re[0] = r.x;
			re[1] = point<T>(r.y.x, r.x.y);
			re[2] = r.y;
			re[3] = point<T>(r.x.x, r.y.y);
			return;
		}

		/// Checks if r is inside rc
		inline bool RectInRect(const Rect& rc, const Rect& r)
		{
				if (
					PointInRect(rc, r.x)
					&&
					PointInRect(rc, r.y)
					)
					return true;
			return false;
		}

		/// Checks if two rectangles overlap
		inline bool rcOverlap(const Rect& lhs, const Rect& rhs)
		{
			// check if they _don't_ overlap
			if (
					// lhs right of rhs
				   lhs.x.x > rhs.y.x
					// lhs left of rhs
				|| lhs.y.x < rhs.x.x
					// lhs below rhs
				|| lhs.x.y > rhs.y.y
					// lhs above rhs
				|| lhs.y.y < rhs.x.y
				)
				return false;
			return true;
		}

		template< class T >
		inline point<T> clip( const point<T> v, const point<point<T>>& limits )
		{
			point<T> vec = v;
			if ( vec.x < limits.x.x ) vec.x = limits.x.x;
			else if ( vec.x > limits.y.x ) vec.x = limits.y.x;

			if ( vec.y < limits.x.y ) vec.y = limits.x.y;
			else if ( vec.y > limits.y.y ) vec.y = limits.y.y;

			return vec;
		}

	//		if (   PointInRect(lhs, rhs.x)
	//			|| PointInRect(lhs, rcTRPoint(rhs))
	//			|| PointInRect(lhs, rhs.y)
	//			|| PointInRect(lhs, rcBLPoint(rhs))
	//
	//			|| PointInRect(rhs, lhs.x)
	//			|| PointInRect(rhs, rcTRPoint(lhs))
	//			|| PointInRect(rhs, lhs.y)
	//			|| PointInRect(rhs, rcBLPoint(lhs))
	//			//|| RectInRect(lhs, rhs)
	//			//|| RectInRect(rhs, lhs)
	//			)
	//		return true;
	//
	//		return false;


		/*
			Some helper functios for "quadifying"/subdividing an area
		*/

		/// Subdivie a rectangle into 4 subrectangles, return top-left one
		inline Rect rcTLQuad(const Rect& r)
		{
			return Rect( //top left point
					 r.x,
					 //bottom right point add half height/width
					 Point( static_cast<int>(r.x.x + 0.5*rcWidth(r)),
							static_cast<int>(r.x.y + 0.5*rcHeight(r)) )
					);
		}

		/// Subdivie a rectangle into 4 subrectangles, return top-right one
		inline Rect rcTRQuad(const Rect& r)
		{
			return Rect( //top left point
					 Point( static_cast<int>(r.x.x + 0.5*rcWidth(r)),
							r.x.y ),
					 //bottom right point
					 Point( r.y.x,
							static_cast<int>(r.x.y + 0.5*rcHeight(r)) )
					);
		}

		/// Subdivie a rectangle into 4 subrectangles, return bottom-right one
		inline Rect rcBRQuad(const Rect& r)
		{
			return Rect( //top left point
					 Point( static_cast<int>(r.x.x + 0.5*rcWidth(r)),
							static_cast<int>(r.x.y + 0.5*rcHeight(r)) ),
					 //bottom right point
					 r.y
					);
		}

		/// Subdivie a rectangle into 4 subrectangles, return bottom-left one
		inline Rect rcBLQuad(const Rect& r)
		{
			return Rect( //top left point
					 Point( r.x.x,
							static_cast<int>( r.x.y + 0.5*rcHeight(r))),
					 //bottom right point add half height/width
					 Point( static_cast<int>(r.x.x + 0.5*rcWidth(r)),
							r.y.y )
					);
		}

	//	inline Point rcMid(const Rect& r)
	//	{
	//		return Point( r.x.x + 0.5*rcWidth(r),
	//					  r.x.y + 0.5*rcHeight(r)
	//				);
	//	}


	} // namespace Geom
} // namespace sbe

#endif // RECTHELPERS_HPP

