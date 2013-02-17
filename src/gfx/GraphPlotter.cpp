#include "sbe/gfx/GraphPlotter.hpp"

#include "sbe/gfx/VertexUtils.hpp"
#include "sbe/Engine.hpp"

#include <cmath>

#include <SFML/Graphics/RenderTarget.hpp>

void GraphPlotter::setGraph ( const Graph& g )
{
	this->g = g;
	RenderArrays.clear();
	Axes.clear();
	Axes.setPrimitiveType(sf::Lines );
	RenderArrays.insert( RenderArrays.end(), 2 + g.Curves.size(), sf::VertexArray() );
	for ( sf::VertexArray& vA : RenderArrays) vA.setPrimitiveType(sf::Lines);
}

void GraphPlotter::updateCurve ( std::string& name, Curve& C )
{
	for ( int i = 0; i < g.Curves.size(); ++i)
	{
		if ( g.Curves[i].name == name )
		{
			g.Curves[i] = C;
			// update the corresponding vertexarray
			drawCurve( g.Curves[i], RenderArrays[i] );
		}
	}
}

void GraphPlotter::updateVertexArrays()
{
	if ( g.drawLegend ) drawLegend();
	if ( g.drawAxes ) drawAxes();

	for ( int i = 0; i < g.Curves.size(); ++i)
	{
		drawCurve( g.Curves[i], RenderArrays[i]);
	}
}

void GraphPlotter::draw ( sf::RenderTarget& Target )
{
	Target.clear( sf::Color::White );
	Target.draw( Axes );

	for ( sf::VertexArray& v : RenderArrays )
		Target.draw(v);
}

void GraphPlotter::drawLegend()
{
	// not implemented yet
}

void GraphPlotter::drawAxes()
{
	const int border = 1;

	gfx::AppendLine( Axes, sf::Vector2f( border, g.Size.y-border), sf::Vector2f( g.Size.x, g.Size.y-border ), sf::Color::Black);
	gfx::AppendLine( Axes, sf::Vector2f( border, g.Size.y-border), sf::Vector2f( border, border ), sf::Color::Black);

	/// distance between each Datapoint
	float PointDistance, x,y;


	PointDistance = (float)g.Size.x/ g.AxesPoints.x;

	for ( unsigned int p = 1; p <= g.AxesPoints.x; ++p)
	{
		x = (float)p*PointDistance;
		y = g.Size.y - (p%5==0?10:5);
		gfx::AppendLine( Axes, sf::Vector2f( x, g.Size.y-border), sf::Vector2f(x, y), sf::Color::Black);
	}

	PointDistance = (float)g.Size.y / g.AxesPoints.y;
	for ( unsigned int p = 1; p <= g.AxesPoints.y; ++p)
	{
		y = g.Size.y - (float)p*PointDistance;
		x = p%5==0?10:5;
		gfx::AppendLine( Axes, sf::Vector2f( border, y), sf::Vector2f(x+border, y), sf::Color::Black);
	}

}

void GraphPlotter::drawCurve ( const Curve& c, sf::VertexArray& vA )
{
	/// distance between each Datapoint
	float PointDistance = (float)g.Size.x / g.PointsToDraw;

	// calculate the first point
	Geom::Point last(0, 0);
	Geom::Point current;

	for ( int p = 0; p < g.PointsToDraw; ++p)
	{
		current.x = last.x + PointDistance ;
		float x_percentage = current.x / (float)g.Size.x;
		float y_percentage = interpolatedCurveData( c, x_percentage  ) / (float)g.AxisSize.y;
		// invert for opengls top-left origin
		current.y = g.Size.y* (1 - y_percentage);

		//Engine::out() << "Percentage X:" <<  x_percentage << std::endl;
		//Engine::out() << "Percentage Y:" <<  y_percentage << std::endl;
		//Engine::out() << "Drawing datapoint " << last.x << "/" << last.y << " -- " << current.x << "/" << current.y  << std::endl;
		//Engine::out() << "Value is: " << interpolatedCurveData( c, x_percentage ) << std::endl;

		if (last.x == 0 && last.y == 0) last = current;

		gfx::AppendLine( vA, sf::Vector2f(last.x, last.y), sf::Vector2f(current.x, current.y), c.color);
		last = current;
	}
}

int GraphPlotter::interpolatedCurveData( const Curve& c, float percentage)
{

	if ( percentage > 1.0 ) percentage = 1;
	if ( percentage < 0 ) percentage = 0;

	// determine matching indizes
	int from = std::floor( percentage*c.data.size() );
	int to   = std::ceil ( percentage*c.data.size() );

	//float unused;
	//float per = std::modf( percentage*c.data.size(), &unused );
	float per = percentage*c.data.size() - (int)(percentage*c.data.size());

	//Engine::out() << "c.data[" << from << "]" << c.data[from] << std::endl;
	//Engine::out() << "c.data["<< to <<"]" << c.data[to] << std::endl;
	//Engine::out() << "percentage" << percentage << " - per: " << per << std::endl;

	return c.data[from] + ( c.data[to] - c.data[from] )*per;

}
