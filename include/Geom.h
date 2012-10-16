#ifndef GEOM_H
#define GEOM_H


#include <boost/operators.hpp>

#include <boost/lexical_cast.hpp>

#include <SFML/Graphics/Rect.hpp>

//#include "Engine.h"

#include <cmath>
#include <iostream>

namespace Geom
{

	template <class T>
	class point    // note: private inheritance is OK here!
		: boost::addable< point<T>          // point + point
		, boost::subtractable< point<T>     // point - point
		, boost::dividable2< point<T>, T    // point / T
		, boost::multipliable2< point<T>, T // point * T, T * point
		  > > > >
	{

		public:
			point(T x, T y) : x_(x), y_(y) {};
			point() : x_(), y_() {};

			T x() const {return x_;};
			T y() const {return y_;};

			point operator+=(const point& p) { x_ += p.x_; y_ += p.y_; return *this; }
			// point operator+(point, const point&) automatically
			// generated by addable.

			point operator-=(const point& p) { x_ -= p.x_; y_ -= p.y_; return *this;}
			// point operator-(point, const point&) automatically
			// generated by subtractable.

			point operator*=(T);// { x_ *= p.x_; y_ *= p.y_; }
			// point operator*(point, const T&) and
			// point operator*(const T&, point) auto-generated
			// by multipliable.

			point operator/=(T);// { x_ /= p.x_; y_ /= p.y_; }
			// point operator/(point, const T&) auto-generated
			// by dividable.

			void SetX(T X) { x_ = X; };
			void SetY(T Y) { y_ = Y; };

			std::string ToString()
			{
				 return boost::lexical_cast<std::string>(x_)
						+ '/' +
						boost::lexical_cast<std::string>(y_);
			}

		private:
			T x_;
			T y_;
	};




	/*
	template <>
	class point<int    // note: private inheritance is OK here!
		: boost::addable< point<T>          // point + point
		, boost::subtractable< point<T>     // point - point
		, boost::dividable2< point<T>, T    // point / T
		, boost::multipliable2< point<T>, T // point * T, T * point
		  > > > >
	{

		public:
			point(T, T);
			point() : x_(), y_() {};

			T x() const;
			T y() const;

			point operator+=(const point&);
			// point operator+(point, const point&) automatically
			// generated by addable.

			point operator-=(const point&);
			// point operator-(point, const point&) automatically
			// generated by subtractable.

			point operator*=(T);
			// point operator*(point, const T&) and
			// point operator*(const T&, point) auto-generated
			// by multipliable.

			point operator/=(T);
			// point operator/(point, const T&) auto-generated
			// by dividable.

			void SetX(T X) { x_ = X; };
			void SetY(T Y) { y_ = Y; };

		private:
			T x_;
			T y_;
	};
	*/

	typedef point<int> Point;
	typedef Point Vec2;

	/// a Rect consists of 2 points, top-left and bottom-right position of the rect
	typedef point<Point> Rect;

	template <typename T>
	std::ostream &operator<<(std::ostream &stream, point<T> ob)
	{
	  stream << ob.x() << '/' << ob.y() ;

	  return stream;
	}


	inline std::string RectToString( const Geom::Rect& r)
	{
		std::string re("");
		re += "[";
		re += boost::lexical_cast<std::string>(r.x().x());
		re += "/";
		re += boost::lexical_cast<std::string>(r.x().y());
		re += "|";
		re += boost::lexical_cast<std::string>(r.y().x());
		re += "/";
		re += boost::lexical_cast<std::string>(r.y().y());
		re += "]";

		return re;
	}













	inline Point Origin() { return Point(0,0); }





	inline bool PointInRect(const Rect& rc, const Point& p)
	{
			if (
				(p.x() >= rc.x().x() && p.x() <= rc.y().x())
				&&
				(p.y() >= rc.x().y() && p.y() <= rc.y().y())
				)
				return true;
		return false;
	}



	inline int nextPow2(int x)
	{
		//int tmp = x;
		int y = x;

		while ( x &= ( x ^ ( ~x + 1 ) ) )
			y = x << 1;

		//Engine::out() << "next pow2 to " << tmp << " is " << y << std::endl;


		return y;
	}


//		// http://acius2.blogspot.com/2007/11/calculating-next-power-of-2.html
//		int powof2 = 1;
//		// Double powof2 until >= val
//		while( powof2 < val ) powof2 <<= 1;
//
//		return powof2;
//	}



	inline int rcWidth(const Rect& rc)
	{
		return abs(rc.y().x() - rc.x().x());
	}

	inline int rcHeight(const Rect& rc)
	{
		return abs(rc.y().y() - rc.x().y());
	}

	inline Point rcBLPoint(const Rect& rc)
	{
		return Geom::Point(rc.x().x(), rc.y().y());
	}

	// returns the top right point of a rect, computed from TL-point + width
	inline Point rcTRPoint(const Rect& rc)
	{
		return Geom::Point(rc.y().x(), rc.x().y());
	}



	inline Rect makeRect(int left, int top, int right, int bottom)
	{
		return Rect( Point(left, top), Point(right, bottom) );
	}

	/// is r in r
	inline bool RectInRect(const Rect& rc, const Rect& r)
	{
			if (
				PointInRect(rc, r.x())
				&&
				PointInRect(rc, r.y())
				)
				return true;
		return false;
	}


	inline bool rcOverlap(const Rect& lhs, const Rect& rhs)
	{
		// check if the don't overlap
		if (
				// lhs right of rhs
			   lhs.x().x() > rhs.y().x()
				// lhs left of rhs
			|| lhs.y().x() < rhs.x().x()
				// lhs below rhs
			|| lhs.x().y() > rhs.y().y()
				// lhs above rhs
			|| lhs.y().y() < rhs.x().y()
			)
			return false;
		return true;


//		if (   PointInRect(lhs, rhs.x())
//			|| PointInRect(lhs, rcTRPoint(rhs))
//			|| PointInRect(lhs, rhs.y())
//			|| PointInRect(lhs, rcBLPoint(rhs))
//
//			|| PointInRect(rhs, lhs.x())
//			|| PointInRect(rhs, rcTRPoint(lhs))
//			|| PointInRect(rhs, lhs.y())
//			|| PointInRect(rhs, rcBLPoint(lhs))
//			//|| RectInRect(lhs, rhs)
//			//|| RectInRect(rhs, lhs)
//			)
//		return true;
//
//		return false;
	}

		/**
	Some helper functios for "quadifying" an area
	*/

	/// return the top left quad for a given rectangle
	inline Rect rcTLQuad(const Rect& r)
	{
		return Rect( //top left point
				 r.x(),
				 //bottom right point add half height/width
				 Point( r.x().x() + 0.5*rcWidth(r), r.x().y() + 0.5*rcHeight(r) )
				);
	}

	/// top right
	inline Rect rcTRQuad(const Rect& r)
	{
		return Rect( //top left point
				 Point( r.x().x() + 0.5*rcWidth(r), r.x().y() ),
				 //bottom right point
				 Point( r.y().x(), r.x().y() + 0.5*rcHeight(r) )
				);
	}

	/// bottom right
	inline Rect rcBRQuad(const Rect& r)
	{
		return Rect( //top left point
				 Point( r.x().x() + 0.5*rcWidth(r), r.x().y() + 0.5*rcHeight(r) ),
				 //bottom right point
				 r.y()
				);
	}

	/// bottom left
	inline Rect rcBLQuad(const Rect& r)
	{
		return Rect( //top left point
				 Point( r.x().x(), r.x().y() + 0.5*rcHeight(r)),
				 //bottom right point add half height/width
				 Point( r.x().x() + 0.5*rcWidth(r), r.y().y() )
				);
	}

//	inline Point rcMid(const Rect& r)
//	{
//		return Point( r.x().x() + 0.5*rcWidth(r),
//					  r.x().y() + 0.5*rcHeight(r)
//				);
//	}







	/// calculates the distance between two points (just simple and not optimized)
	inline float distance(const Point& lhs, const Point& rhs)
	{
		return sqrt(
					pow(static_cast<int>(abs(lhs.x())) + abs(rhs.x()), 2)
					+
					pow(static_cast<int>(abs(lhs.x())) + abs(rhs.x()), 2)
					);
	}

	inline Geom::Point middle(const Point& p1, const Point& p2)
	{
		return Geom::Point(
							abs(p1.x() + p2.x())/2,
							abs(p1.y() + p2.y())/2
							);
	}

	inline sf::IntRect toSFRect(const Rect& r)
	{
		return sf::IntRect( r.x().x(), // left
							r.x().y(), // top
							r.y().x()-r.x().x(), // width
							r.y().y()-r.x().y() ); // height
	}

	inline sf::IntRect SFRectFloatToInt(const sf::FloatRect& r)
	{
		sf::IntRect re ( static_cast<int>(r.left),
						 static_cast<int>(r.top),
						 static_cast<int>(r.width),
						 static_cast<int>(r.height)
						);

		//Engine::out() << "float:" << r.Left << " / " << r.Top << " / " << r.Right << " / " << r.Bottom << std::endl;
		//Engine::out() << "int:" << re.Left << " / " << re.Top << " / " << re.Right << " / " << re.Bottom << std::endl;
		return re;
	}

	inline Rect fromSFRect(const sf::IntRect& r)
	{
		return Rect(Point(r.left, r.top), Point(r.left + r.width, r.top + r.height));
	}

	inline Rect fromSFFloatRect ( const sf::FloatRect& r )
	{
		return fromSFRect( SFRectFloatToInt(r) );
	}

	// TRIANGLE

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
		return PointInTri(p.x(), p.y(), t1.x(), t1.y(), t2.x(), t2.y(), t3.x(), t3.y());
	}


};	// namespace Geom

#endif // GEOM_H



