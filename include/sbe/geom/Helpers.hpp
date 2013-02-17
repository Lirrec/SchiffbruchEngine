#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <boost/lexical_cast.hpp>

#include <SFML/Graphics/Rect.hpp>

#include <cmath>

#include "sbe/geom/Point.hpp"

namespace Geom
{

	/// Converts a rectangle to a human readable string suitable for debugging purposes
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

	/// returns the origin (0,0)
	inline Point Origin() { return Point(0,0); }
	/// returns the origin (0.0,0.0)
	inline Pointf Originf() { return Pointf(0.0f,0.0f); }


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



	/**
		Calculate the next power of 2 which is greater than x.
		old ref: http://acius2.blogspot.com/2007/11/calculating-next-power-of-2.html
	*/
	inline int nextPow2(int x)
	{
		int y = x;
		while ( x &= ( x ^ ( ~x + 1 ) ) )
			y = x << 1;
		return y;
	}


	/// returns the width of a rectangle
	inline int rcWidth(const Rect& rc)
	{
		return abs(rc.y.x - rc.x.x);
	}

	/// returns the height of a rectangle
	inline int rcHeight(const Rect& rc)
	{
		return abs(rc.y.y - rc.x.y);
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
	inline Rect makeRect(int left, int top, int right, int bottom)
	{
		return Rect( Point(left, top), Point(right, bottom) );
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
	}

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







	/// calculates the distance between two points (just simple and not optimized)
	inline double distance(const Point& lhs, const Point& rhs)
	{
		return sqrt( pow(static_cast<int>(abs(lhs.x)) - abs(rhs.x), 2)    +    pow(static_cast<int>(abs(lhs.y)) - abs(rhs.y), 2));
	}

	/// Compute the distance between two points ( floating point version )
	inline double distance(const Pointf& from, const Pointf& to)
	{
		return sqrt( pow(to.x - from.x, 2)    +    pow(to.y - from.y, 2));
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

	/**
		Normalize a float, i.e. scale it to a range from 0.0 to 1.0
		@param f the float to normalize
		@param min the minimum
	*/
	inline float normalize ( float f, float min, float max)
	{
		assert ( max - min != 0 );
		return (f - min)/( max - min );
	}

	/**
		Return the middle point of a line defined by p1 and p2
	*/
	inline Geom::Point middle(const Point& p1, const Point& p2)
	{
		return Geom::Point( static_cast<int>(abs(p1.x + p2.x)/2),    static_cast<int>(abs(p1.y + p2.y)/2));
	}

	/// converts a Geom::Rect to sf::Floatrect
	inline sf::FloatRect toSFFloatRect ( const Rect& r)
	{
		return sf::FloatRect( r.x.x, // left
							r.x.y, // top
							r.y.x-r.x.x, // width
							r.y.y-r.x.y ); // height
	}

	/// converts a Geom::Rect to sf::IntRect
	inline sf::IntRect toSFRect(const Rect& r)
	{
		return sf::IntRect( r.x.x, // left
							r.x.y, // top
							r.y.x-r.x.x, // width
							r.y.y-r.x.y ); // height
	}

	/// converts a sf::FloatRect to sf::IntRect ( ! Loss of fp precision ! )
	inline sf::IntRect SFRectFloatToInt(const sf::FloatRect& r)
	{
		sf::IntRect re ( static_cast<int>(r.left),
						 static_cast<int>(r.top),
						 static_cast<int>(r.width),
						 static_cast<int>(r.height)
						);
		return re;
	}

	/// converts a sf::IntRect to Geom::Rect
	inline Rect fromSFRect(const sf::IntRect& r)
	{
		return Rect(Point(r.left, r.top), Point(r.left + r.width, r.top + r.height));
	}

	/// converts a sf::FloatRect to Geom::Rect ( ! Loss of fp precision ! )
	inline Rect fromSFFloatRect ( const sf::FloatRect& r )
	{
		return fromSFRect( SFRectFloatToInt(r) );
	}

	// TRIANGLE
	/**
		Checks wether a given point is inside a triangle
		@see PointInTri(Point p, Point t1, Point t2, Point t3) a version which takes points as parameters
	*/
	inline bool PointInTri(int X, int Y, int X0, int Y0, int X1, int Y1, int /*X3*/, int Y3)
	{
		float	x,y,
				x0,y0,
				x1,y1,
				//x3,
				y3,
				a,b,c,d;

		x=(float)X;
		y=(float)Y;

		x0=(float)X0;
		y0=(float)Y0;

		x1=(float)X1;
		y1=(float)Y1;

		//x3=(float)X3;
		y3=(float)Y3;

		c= (x-x1)/(x0-x1);
		if (c<0) return false;
		d= ((y-y3)*(x0-x1)-(x-x1)*(y0-y3))/((y1-y3)*(x0-x1));
		if (d<0) return false;
		b= ((y-y0)*(x1-x0)-(x-x0)*(y1-y0))/((x1-x0)*(y3-y1));
		if (b<0) return false;
		a= (x-x0)/(x1-x0)-b;
		if (a<0) return false;
		return true;
	}

	inline bool PointInTri(Point p, Point t1, Point t2, Point t3)
	{
		return PointInTri(p.x, p.y, t1.x, t1.y, t2.x, t2.y, t3.x, t3.y);
	}


}	// namespace Geom

#endif // HELPERS_HPP