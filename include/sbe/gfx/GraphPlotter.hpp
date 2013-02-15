#ifndef GRAPHPLOT_HPP
#define GRAPHPLOT_HPP

#include "sbe/geom/Point.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <string>
#include <vector>

class Curve
{
	public:
	Curve( const std::string& _name, const std::vector<int> _data, const sf::Color& _color)
	: name( _name), data(_data), color(_color)
	{}
	std::string name;
	std::vector<int> data;
	sf::Color color;
};

class Graph
{
public:

	Graph()
	 : Size( 512,512),
	 AxisSize( 1000, 1000),
	 //AxisStart ( 0, 0 ),
	 PointsToDraw( 50 ),
	 AxesPoints ( 10, 10),
	 drawLegend(false),
	 drawAxes(true)
	{

	}

	std::vector< Curve > Curves;
	Geom::Vec2 Size;
	/**
		Determines the range of the x and y axes ( e.g. start -> start + AxesSize.x )
	*/
	Geom::Vec2 AxisSize;
	/**
		Determines the first point on each axis to show ( usually 0,0 )
	*/
	//Geom::Vec2 AxisStart;

	/** determines the granularity of the graph ( default 50 ) */
	unsigned int PointsToDraw;

	/**
		how many visual markers should be placed on the axes ( x/y) (defautl: 10/10)
	*/
	Geom::Vec2 AxesPoints;


	bool drawLegend;
	bool drawAxes;
};

class GraphPlotter
{
	public:
	GraphPlotter() {}

	/**
		Set the graph to be plotted
	*/
	void setGraph( const Graph& g );
	Graph& getGraph() { return g; }
	/**
		Update a single curve of the Graph
	*/
	void updateCurve( std::string& name, Curve& C );

	void updateVertexArrays();
	void draw( sf::RenderTarget& Target );



	private:
	/**
		draw a legend on the graph, not yet implemented
	*/
	void drawLegend();
	void drawAxes();
	void drawCurve( const Curve& c, sf::VertexArray& vA  );

	int interpolatedCurveData( const Curve& c, float percentage);

	Graph g;

	sf::VertexArray Axes;

	/// contains vertices for drawing the indiviual curves
	std::vector<sf::VertexArray> RenderArrays;



};

#endif // GRAPHPLOT_HPP

