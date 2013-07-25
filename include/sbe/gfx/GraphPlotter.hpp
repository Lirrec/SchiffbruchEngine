#ifndef GRAPHPLOT_HPP
#define GRAPHPLOT_HPP

#include "sbe/geom/Point.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Text.hpp>

#include <boost/thread/mutex.hpp>

#include <string>
#include <vector>
#include <list>

namespace sbe
{
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
		 AxisStart ( 0, 0 ),
		 MinPointDist( 1 ),
		 AxesPoints ( 5, 5),
		 drawAxisLabels(true),
		 drawLegend(true),
		 textSize( 15 ),
		 drawAxes(true),
		 dynX(true), dynY(true),
		 logScale(true),
		 logBase(10)
		{

		}

		/** Add a new Curve to the graph.
			@return false if the curve is empty, true if it was added
		*/
		bool addCurve ( const Curve& c );

		std::vector< Curve > getCurves() { return Curves; }

		Geom::Point getMaximas() {
			Geom::Point max;
			for( auto& C : Curves )
			{
				if ( C.data.empty() ) continue;
				if ( C.data.size() > max.x ) max.x = C.data.size();
				float tmp = *std::max_element( C.data.begin(), C.data.end() );
				if ( tmp > max.y ) max.y = tmp;
			}
			return max;
		}

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
		Geom::Vec2 AxisStart;

		/// determines the granularity of the graph or how many points should be rendered ( default 2px distance between each rendered point )
		float MinPointDist;

		/**
			how many visual markers should be placed on the axes ( x/y) (defautl: 10/10)
		*/
		Geom::Vec2 AxesPoints;

		/// wether to draw the labels on the axes
		bool drawAxisLabels;
		/// wether to draw a legend
		bool drawLegend;
		/// character size of the legend text in pixels
		int textSize;
		/// wether to draw the axes
		bool drawAxes;
		/// wether axes should scale
		bool dynX, dynY;
		/// wether Y Axis should scale logarithmic
		bool logScale;
		/// to which base should the y axis scale if logarithmic scaling is enabled
		float logBase;
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
			Update a curve of the Graph
		*/
		void updateCurve( const std::string& name, Curve& C );
		/**
			Update the data of a curve of the Graph
		*/
		void updateCurveData( const std::string& name, std::vector<int>& Data );
		/**
			Add data to a curve of the Graph
		*/
		void extendCurve( const std::string& name, std::vector<int>& Data );
		void extendCurve( const std::string& name, int D );

		/**
			Create the vertexarrays and sprites needed for rendering, has to be called before draw()
		*/
		void updateVertexArrays();
		/**
			Draw the graph to a rendertarget.
			As you cant set a relative origin on the rendertarget yet, this works best with a sf::RenderTexture  of appropriate size or with a sf::View
		*/
		void draw( sf::RenderTarget& Target );

		/**
			Prints current Graph settings to INFO log.
		*/
		void printSettings();

		private:

		/**
			Check if dynamic axes are needed and adjust accordingly
		*/
		void dynScaleAxes( const Geom::Point& max);

		/**
			draw the names of the curves on the graph
		*/
		void drawLegend();
		/**
			draw labels on the axes
		*/
		void drawAxisLabels();
		void drawText( const sf::Vector2f& pos, const std::string& text, bool xAxis);
		void drawAxes();
		void drawCurve( const Curve& c, sf::VertexArray& vA  );

		int interpolatedCurveData( const Curve& c, float percentage);

		bool valid;
		Graph g;

		sf::VertexArray Axes;

		/// contains vertices for drawing the indiviual curves
		std::vector<sf::VertexArray> RenderArrays;
		std::list<sf::Text> AxisLabels;
		std::list<sf::Text> Legend;

		boost::mutex data_mutex;
	};
} // namespace sbe
#endif // GRAPHPLOT_HPP

