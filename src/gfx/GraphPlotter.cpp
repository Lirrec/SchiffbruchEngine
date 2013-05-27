#include "sbe/gfx/GraphPlotter.hpp"

#include "sbe/gfx/VertexUtils.hpp"
#include "sbe/Engine.hpp"
#include "sbe/ResourceManager.hpp"

#include <boost/thread.hpp>

#include <cmath>

#include <SFML/Graphics/RenderTarget.hpp>

namespace sbe
{

	bool Graph::addCurve( const Curve& c)
	{
		Curves.push_back ( c );
		return true;
	}

	//##############################################################
	//##############################################################
	//##############################################################

	GraphPlotter::GraphPlotter()
	{
		valid = false;
	}

	bool GraphPlotter::setGraph ( const Graph& _g )
	{
		if ( _g.Curves.empty() )
		{
			Engine::out(Engine::ERROR) << "[GraphPlotter] Graph has no Curves!" << std::endl;
			return false;
		}

		boost::mutex::scoped_lock data_mutex_lock(data_mutex);

		valid = true;
		g = _g;
		RenderArrays.clear();
		Legend.clear();
		Axes.clear();
		Axes.setPrimitiveType(sf::Lines );
		RenderArrays.insert( RenderArrays.end(), 2 + g.Curves.size(), sf::VertexArray() );
		for ( sf::VertexArray& vA : RenderArrays) vA.setPrimitiveType(sf::Lines);

		return true;
	}

	void GraphPlotter::updateCurve ( const std::string& name, Curve& C )
	{
		if (!valid)
		{
			Engine::out(Engine::ERROR) << "[GraphPlotter] no valid graph set!" << std::endl;
			return;
		}
		boost::mutex::scoped_lock data_mutex_lock(data_mutex);

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

	void GraphPlotter::updateCurveData( const std::string& name, std::vector<int>& Data )
	{
		if (!valid)
		{
			Engine::out(Engine::ERROR) << "[GraphPlotter] no valid graph set!" << std::endl;
			return;
		}
		boost::mutex::scoped_lock data_mutex_lock(data_mutex);

		for ( int i = 0; i < g.Curves.size(); ++i)
		{

			if ( g.Curves[i].name == name )
			{
				g.Curves[i].data = Data;
				// update the corresponding vertexarray
				drawCurve( g.Curves[i], RenderArrays[i] );
			}
		}
	}

	void GraphPlotter::extendCurve( const std::string& name, std::vector<int>& Data )
	{
		if (!valid)
		{
			Engine::out(Engine::ERROR) << "[GraphPlotter] no valid graph set!" << std::endl;
			return;
		}
		boost::mutex::scoped_lock data_mutex_lock(data_mutex);

		for ( int i = 0; i < g.Curves.size(); ++i)
		{
			if ( g.Curves[i].name == name )
			{
				g.Curves[i].data.insert( g.Curves[i].data.end(), Data.begin(), Data.end() );
				// update the corresponding vertexarray
				drawCurve( g.Curves[i], RenderArrays[i] );
			}
		}
	}

	void GraphPlotter::extendCurve( const std::string& name, int D )
	{
		if (!valid)
		{
			Engine::out(Engine::ERROR) << "[GraphPlotter] no valid graph set!" << std::endl;
			return;
		}
		boost::mutex::scoped_lock data_mutex_lock(data_mutex);

		for ( int i = 0; i < g.Curves.size(); ++i)
		{
			if ( g.Curves[i].name == name )
			{
				g.Curves[i].data.push_back(D);
				// update the corresponding vertexarray
				drawCurve( g.Curves[i], RenderArrays[i] );
			}
		}
	}

	void GraphPlotter::updateVertexArrays()
	{
		if (!valid)
		{
			Engine::out(Engine::ERROR) << "[GraphPlotter] no valid graph set!" << std::endl;
			return;
		}

		boost::mutex::scoped_lock data_mutex_lock(data_mutex);

		dynScaleAxes();

		if ( g.drawLegend ) drawLegend();
		if ( g.drawAxes ) drawAxes();

		for ( int i = 0; i < g.Curves.size(); ++i)
		{
			drawCurve( g.Curves[i], RenderArrays[i]);
		}
	}

	void GraphPlotter::dynScaleAxes()
	{

		std::vector<Curve> cs = g.Curves;
		if(g.dynX)
		{
			float maxX = 0;
			for(int i = 0; i<cs.size(); i++)
				if(cs[i].data.size() > maxX) maxX = cs[i].data.size();

			g.AxisSize.x = maxX-g.AxisStart.x;
		}
		if(g.dynY)
		{
			float maxY = 0;
			for(int i = 0; i<cs.size(); i++)
			{
				float tmp = *std::max_element(cs[i].data.begin(), cs[i].data.end());
				if (tmp > maxY) maxY = tmp;
			}
			g.AxisSize.y = maxY-g.AxisStart.y;
		}
	}

	void GraphPlotter::draw ( sf::RenderTarget& Target )
	{
		if (!valid)
		{
			Engine::out(Engine::ERROR) << "[GraphPlotter] no valid graph set!" << std::endl;
			return;
		}

		Target.clear( sf::Color::White );

		if ( g.drawAxes ) Target.draw( Axes );

		for ( sf::VertexArray& v : RenderArrays )
			Target.draw(v);

		if ( g.drawLegend )
		{
			for ( sf::Text& t : Legend)
			{
				Target.draw(t);
			}
		}
	}

	void GraphPlotter::drawLegend()
	{
		const int border = 1;
		/// spacing between the end of the marker on the axis and the position of the text
		const int spacing = 5;
		/// distance between each Datapoint
		float PointDistance, x,y;
		PointDistance = (float)g.Size.x/ g.AxesPoints.x;

		for ( unsigned int p = 1; p <= g.AxesPoints.x; ++p)
		{
			x = (float)p*PointDistance;
			y = g.Size.y - (p%5==0?10:5);
			drawText( sf::Vector2f(x, y - spacing), boost::lexical_cast<std::string>( g.AxisSize.x/g.AxesPoints.x * p ), true );
			//Engine::out(Engine::SPAM) << " Label " << g.AxisSize.x/g.AxesPoints.x * p << " - at: " << x << "," << y-spacing << std::endl;
		}

		PointDistance = (float)g.Size.y / g.AxesPoints.y;
		for ( unsigned int p = 1; p <= g.AxesPoints.y; ++p)
		{
			y = g.Size.y - (float)p*PointDistance;
			x = p%5==0?10:5;

			drawText( sf::Vector2f(x+border+ spacing, y), boost::lexical_cast<std::string>( g.AxisSize.y/g.AxesPoints.y * p ), false );
			//Engine::out(Engine::SPAM) << " Label " << g.AxisSize.y/g.AxesPoints.y * p << " - at: " << x+border+ spacing << "," << y << std::endl;
		}
	}

	void GraphPlotter::drawText( const sf::Vector2f& pos, const std::string& text, bool xAxis)
	{
		sf::Text t;
		t.setColor( sf::Color::Black );
		t.setString( text );
		t.setFont( *(Engine::GetResMgr()->get<sf::Font>("default")) );
		t.setCharacterSize( g.textSize );
		sf::FloatRect bounds = t.getLocalBounds();
		// center the origin accordingly so we can move it by its centerpoint
		// x axis labels, bottom-mid; y axis labels left-mid
		if ( xAxis) t.setOrigin( bounds.width/2, bounds.height );
		else		t.setOrigin( 0, bounds.height/2 );
		t.setPosition( pos );
		Legend.push_back( t );
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
		float PointDistance = (float)g.Size.x / (float)g.PointsToDraw;

		// calculate the first point
		Geom::Pointf last(0, 0);
		Geom::Pointf current;

		for ( int p = 0; p < g.PointsToDraw; ++p)
		{
			current.x = last.x + PointDistance ;
			if ( current.x > g.Size.x ) current.x = g.Size.x;

			float x_percentage = current.x / (float)g.Size.x;
			if ( g.AxisStart.x != -1 ) x_percentage = (g.AxisStart.x +  x_percentage * (g.AxisSize.x)) / c.data.size();

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

		if ( percentage >= 1.0 ) return ( c.data.empty() ? 0 : c.data.back() );
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
} // namespace sbe
