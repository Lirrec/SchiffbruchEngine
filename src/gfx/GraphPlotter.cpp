#include "sbe/gfx/GraphPlotter.hpp"

#include "sbe/gfx/VertexUtils.hpp"
#include "sbe/Engine.hpp"
#include "sbe/ResourceManager.hpp"

#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include <cmath>
#include <glm/gtx/string_cast.hpp>
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
		AxisLabels.clear();
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

				dynScaleAxes( g.getMaximas() );
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

				dynScaleAxes(g.getMaximas());
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

				dynScaleAxes(g.getMaximas());
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

				dynScaleAxes(g.getMaximas());
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

		if ( g.AxisSize.x <= 0 || g.AxisSize.y <= 0)
		{ Engine::out(Engine::ERROR) << "[GraphPlotter] Invalid Settings ( AxisSize < 0 ) !" << std::endl; return; }

		if ( g.AxesPoints.x <= 0 || g.AxesPoints.y <= 0 )
		{ Engine::out(Engine::ERROR) << "[GraphPlotter] Invalid Settings ( AxesPoints < 0 ) !" << std::endl; return; }

		if ( g.MinPointDist <= 0 )
		{ Engine::out(Engine::ERROR) << "[GraphPlotter] Invalid Settings ( MinPointDist < 0 ) !" << std::endl; return; }

		if ( g.Size.x <= 0 || g.Size.y <= 0  )
		{ Engine::out(Engine::ERROR) << "[GraphPlotter] Invalid Settings ( Size < 0 ) !" << std::endl; return; }

		RenderArrays.clear();
		AxisLabels.clear();
		Legend.clear();
		Axes.clear();
		Axes.setPrimitiveType(sf::Lines );
		RenderArrays.insert( RenderArrays.end(), 2 + g.Curves.size(), sf::VertexArray() );
		for ( sf::VertexArray& vA : RenderArrays) { vA.clear(); vA.setPrimitiveType(sf::Lines); }

		boost::mutex::scoped_lock data_mutex_lock(data_mutex);

		glm::ipoint2 maximas = g.getMaximas();

		dynScaleAxes( maximas );
		if ( g.AxisStart.x > maximas.x) g.AxisStart.x = maximas.x;
		//if ( g.AxisStart.x < 0) g.AxisStart.x = maximas.x + g.AxisStart.x;

		if ( g.AxisStart.y > maximas.y) g.AxisStart.y = maximas.y;
		//if ( g.AxisStart.y < 0) g.AxisStart.y = maximas.y + g.AxisStart.y;


		if ( g.AxisSize.x <= 0 || g.AxisSize.y <= 0)
		{
			Engine::out(Engine::ERROR) << "[GraphPlotter] Invalid Settings ( AxisSize < 0 ) !" << std::endl;
			//printSettings();
			return;
		}

		if ( g.drawLegend ) drawLegend();
		if ( g.drawAxisLabels ) drawAxisLabels();
		if ( g.drawAxes ) drawAxes();

		for ( int i = 0; i < g.Curves.size(); ++i)
		{
			drawCurve( g.Curves[i], RenderArrays[i]);
		}
	}

	void GraphPlotter::dynScaleAxes( const glm::ipoint2& max)
	{
		if(!g.dynX && !g.dynY) return;

		std::vector<Curve>& cs = g.Curves;

		if ( g.dynX )
		{
			g.AxisStart.x = 0;
			g.AxisSize.x = max.x-g.AxisStart.x;
		}
		if ( g.dynY )
		{
			g.AxisStart.y = 0;
			g.AxisSize.y = max.y-g.AxisStart.y;
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
		dynScaleAxes( g.getMaximas() );

		if ( g.drawAxes ) Target.draw( Axes );

		for ( sf::VertexArray& v : RenderArrays )
			Target.draw(v);

		if ( g.drawAxisLabels )
			for ( sf::Text& t : AxisLabels)
				Target.draw(t);

		if ( g.drawLegend )
			for ( sf::Text& t : Legend)
				Target.draw(t);
	}

	void GraphPlotter::drawAxisLabels()
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
			int label = g.AxisStart.x + (g.AxisSize.x/g.AxesPoints.x * p);
			drawText( sf::Vector2f(x, y - spacing), boost::lexical_cast<std::string>( label ), true );
			//Engine::out(Engine::SPAM) << " Label " << g.AxisSize.x/g.AxesPoints.x * p << " - at: " << x << "," << y-spacing << std::endl;
		}

		PointDistance = (float)g.Size.y / g.AxesPoints.y;
		for ( unsigned int p = 1; p <= g.AxesPoints.y; ++p)
		{
			y = g.Size.y - (float)p*PointDistance;
			x = p%5==0?10:5;

			int label;
			if ( !g.logScale ) label =  g.AxisStart.y + (g.AxisSize.y/g.AxesPoints.y * p);
			else
			{
				/**
					percentage = log10( Y ) / log10( g.AxisSize.y );
					=> percentage * log10( g.AxisSize.y ) = log10( Y );
					=> std::pow( 10, percentage * log10( g.AxisSize.y )) = Y;
				*/
				label = std::pow( g.logBase, ( p / (float)g.AxesPoints.y ) * (std::log( g.AxisSize.y ) / std::log(g.logBase) ) );
			}

			drawText( sf::Vector2f(x+border+ spacing, y), boost::lexical_cast<std::string>( label ), false );
			//Engine::out(Engine::SPAM) << " Label " << g.AxisSize.y/g.AxesPoints.y * p << " - at: " << x+border+ spacing << "," << y << std::endl;
		}
	}

	void GraphPlotter::drawLegend()
	{
		/// spacing between each label
		const int spacing = 5;
		int y = 5;
		int maxwidth = 0;


		for ( Curve& c : g.Curves )
		{
			sf::Text t;
			t.setColor( c.color );
			t.setString( c.name );
			t.setFont( *(Engine::GetResMgr()->get<sf::Font>("default")) );
			t.setCharacterSize( g.textSize );
			sf::FloatRect bounds = t.getLocalBounds();
			t.setOrigin( 0, 0 );
			y += spacing + bounds.height;
			if ( bounds.width > maxwidth ) maxwidth = bounds.width;
			t.setPosition( 0, y );
			Legend.push_back( t );
		}

		for ( sf::Text& t : Legend)
			t.setPosition( g.Size.x - maxwidth - 5, t.getPosition().y );
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
		AxisLabels.push_back( t );
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
		if ( c.data.empty() || g.AxisSize.x <= 0 || g.AxisSize.y <= 0 ) return;

		/// distance between each Datapoint
		float validwidth = ((float)c.data.size() / (float)g.AxisSize.x) * g.Size.x;
		if ( validwidth > g.Size.x ) validwidth = g.Size.x;

		int PointsToDraw = (validwidth / g.MinPointDist) + 1;

		// calculate the first point
		glm::point2 last(0, 0);
		glm::point2 current;

		for ( int p = 0; p < PointsToDraw; ++p)
		{
			current.x = last.x + g.MinPointDist;
			if ( current.x > validwidth ) current.x = validwidth;
/*
			Engine::out() << "points:" << p << "/" << PointsToDraw << std::endl;
			Engine::out() << "current X:" << current.x << std::endl;
			Engine::out() << "size X:" << g.Size.x << std::endl;
			Engine::out() << "validwidth:" << validwidth << std::endl;
			Engine::out() << "g.AxisStart.x:" <<  g.AxisStart.x << std::endl;
			Engine::out() << "g.AxisSize.x:" <<  g.AxisSize.x << std::endl;
			Engine::out() << "c.data.size():" <<  c.data.size() << std::endl;
*/
			float x_percentage = current.x / (float)g.Size.x;
			x_percentage = ((float)g.AxisStart.x +  x_percentage * (float)g.AxisSize.x) / c.data.size();

			float y_percentage;
			if ( !g.logScale ) y_percentage = (interpolatedCurveData( c, x_percentage  ) - g.AxisStart.y) / (float)g.AxisSize.y;
			else y_percentage = (std::log( (interpolatedCurveData( c, x_percentage  ) - g.AxisStart.y) ) / std::log(g.logBase) ) / (std::log((float)g.AxisSize.y) / std::log(g.logBase) );
			// invert for opengls top-left origin
			current.y = g.Size.y * (1 - y_percentage);

/*
			Engine::out() << "Percentage X:" <<  x_percentage << std::endl;
			Engine::out() << "Percentage Y:" <<  y_percentage << std::endl;
			Engine::out() << "Drawing datapoint " << last.x << "/" << last.y << " -- " << current.x << "/" << current.y  << std::endl;
			Engine::out() << "Value is: " << interpolatedCurveData( c, x_percentage ) << std::endl;
*/
			if (last.x == 0 && last.y == 0) last = current;

			if ( y_percentage > 1 || x_percentage > 1 ) continue;
			gfx::AppendLine( vA, sf::Vector2f(last.x, last.y), sf::Vector2f(current.x, current.y), c.color);
			last = current;
		}
	}

	int GraphPlotter::interpolatedCurveData( const Curve& c, float percentage)
	{
		if ( c.data.empty() ) return 0;
		if ( percentage >= 1.0 ) return c.data.back();
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

	void GraphPlotter::printSettings()
	{
		Engine::out(Engine::INFO) << "[GraphPlotter] Settings:" << std::endl;
		Engine::out(Engine::INFO) << "[GraphPlotter] Dynamic x/y:" << g.dynX << "/" << g.dynY << std::endl;
		Engine::out(Engine::INFO) << "[GraphPlotter] AxisSize: " << glm::to_string(g.AxisSize) << std::endl;
		Engine::out(Engine::INFO) << "[GraphPlotter] AxisStart: " << glm::to_string(g.AxisStart) << std::endl;
		Engine::out(Engine::INFO) << "[GraphPlotter] AxesPoints: " << glm::to_string(g.AxesPoints) << std::endl;
		Engine::out(Engine::INFO) << "[GraphPlotter] Size: " << glm::to_string(g.Size) << std::endl;
	}
} // namespace sbe
