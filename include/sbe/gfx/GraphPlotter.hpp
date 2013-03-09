#ifndef GRAPHPLOT_HPP
#define GRAPHPLOT_HPP

#include "sbe/geom/Point.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Text.hpp>

#include <string>
#include <vector>
#include <list>

/**
	Struct representing a curve in a Graph.
*/
class Curve
{
	public:
	Curve( const std::string& _name, const std::vector<int> _data, const sf::Color& _color)
	: name( _name), data(_data), color(_color)
	{}
	/// the name of the Curve
	std::string name;
	/// a vector of datapoints
	std::vector<int> data;
	/// the color of the curve
	sf::Color color;

};

/**
	A Graph.
	This is a Graph with one or more curves.
*/
class Graph
{
public:

	Graph()
	 : Size( 512,512),
	 AxisSize( 1000, 1000),
	 //AxisStart ( 0, 0 ),
	 PointsToDraw( 50 ),
	 AxesPoints ( 10, 10),
	 drawLegend(true),
	 textSize( 15 ),
	 drawAxes(true)
	{

	}

	/** Add a new Curve to the graph.
		@return false if the curve is empty, true if it was added
	*/
	bool addCurve ( const Curve& c );

	/**
		The Size of the final image to which this graph should be rendered
	*/
	Geom::Vec2 Size;
	/**
		Determines the range of the x and y axes ( e.g. start -> start + AxesSize.x )
	*/
	Geom::Vec2 AxisSize;

	/**
		Determines the first point on each axis to show ( usually 0,0 )
	*/
	//Geom::Vec2 AxisStart;

	// determines the granularity of the graph or how many points should be rendered ( default 50 )
	unsigned int PointsToDraw;

	/**
		how many visual markers should be placed on the axes ( x/y) (defautl: 10/10)
	*/
	Geom::Vec2 AxesPoints;

	/// wether to draw a legend
	bool drawLegend;
	/// character size of the legend text in pixels
	int textSize;
	/// wether to draw the axes
	bool drawAxes;

	friend class GraphPlotter;
private:
	std::vector< Curve > Curves;
};
/**
	This class can be used to plot simple Graphs with one or more curves in it.
*/
class GraphPlotter
{
	public:
	GraphPlotter();

	/**
		Set the graph to be plotted
	*/
	bool setGraph( const Graph& _g );
	/// access the current graph
	Graph& getGraph() { return g; }

	/// return wether the current graph is valid and can be rendered
	bool isValid() const { return valid; }

	/**
		Update a single curve of the Graph
	*/
	void updateCurve( std::string& name, Curve& C );

	/**
		Create the vertexarrays and sprites needed for rendering, has to be called before draw()
	*/
	void updateVertexArrays();
	/**
		Draw the graph to a rendertarget.
		As you cant set a relative origin on the rendertarget yet, this works best with a sf::RenderTexture  of appropriate size or with a sf::View
	*/
	void draw( sf::RenderTarget& Target );



	private:
	/**
		draw a legend on the graph, not yet implemented
	*/
	void drawLegend();
	void drawText( const sf::Vector2f& pos, const std::string& text, bool xAxis);
	void drawAxes();
	void drawCurve( const Curve& c, sf::VertexArray& vA  );

	int interpolatedCurveData( const Curve& c, float percentage);

	bool valid;
	Graph g;

	sf::VertexArray Axes;

	/// contains vertices for drawing the indiviual curves
	std::vector<sf::VertexArray> RenderArrays;
	std::list<sf::Text> Legend;


};

#endif // GRAPHPLOT_HPP

