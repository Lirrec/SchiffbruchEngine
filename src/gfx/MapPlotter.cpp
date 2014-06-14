#include "sbe/gfx/MapPlotter.hpp"

#include "sbe/Engine.hpp"

#include "sbe/geom/Point.hpp"
#include "sbe/geom/Helpers.hpp"

#include <algorithm>

namespace sbe
{
	MapPlotter::MapPlotter()
	 : Size(0,0),
	 Mode(PLOT_GREYSCALE)
	{

	}

	void MapPlotter::setData(std::vector<float>& data, glm::ivec2 _Size, bool normalize)
	{
		if ( data.size() != _Size.x * _Size.y || _Size.x == 0 || _Size.y == 0)
		{
			Engine::out(Engine::ERROR) << "[MapPlotter] Data has invalid length or doesn't match given size!" << std::endl;
			return;
		}

		Size = _Size;
		Data = data;

		if (normalize)
		{
			float min = *std::min_element( Data.begin(), Data.end());
			float max = *std::max_element( Data.begin(), Data.end());
			for( float& f : Data ) f = Geom::normalize(f, min, max);
		}

		Img.create( Size.x, Size.y );
	}

	void MapPlotter::plot()
	{
		if ( Data.size() != Size.x * Size.y || Size.x == 0 || Size.y == 0)
		{
			Engine::out(Engine::ERROR) << "[MapPlotter] Data has invalid length or doesn't match given size!" << std::endl;
			return;
		}

		for (int y = 0; y < Size.y; ++y)
			for ( int x = 0; x < Size.x; ++x )
				Img.setPixel( x, y, getColor( Data[ Geom::linear(x,y, Size.x ) ] ));
	}

	sf::Color MapPlotter::getColor(float f)
	{
		switch ( Mode )
		{
			case PLOT_GREYSCALE:
				return sf::Color( 255*f, 255*f, 255*f);
				break;
			case PLOT_HEATMAP:
				/*
					gradient: 	rgb		t
							  0,0,255	0
							  0,255,0	0.5
							  255,0,0	1*/
				// relative position in each stage/sector of the gradient
				float r;

				if ( f < 0.5 )
				{
					r = f/.5;
					return sf::Color(0, r*255, (1-r)*255 );
				}
				else if ( f >= 0.5 )
				{
					r = (f-.5)/.5;
					return sf::Color(r*255, (1-r)*255, 0 );
				}

				break;
			default:
				return sf::Color(214,127,255);
				break;
		}
	}
} // namespace sbe
