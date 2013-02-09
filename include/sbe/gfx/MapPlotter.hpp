#ifndef MAPPLOT_H
#define MAPPLOT_H

#include "sbe/Geom.hpp"

#include <vector>
#include <SFML/Graphics/Image.hpp>

class MapPlotter {
public:

	/**
		Available plotting modes.
		These modes define how the normalized input data ( 0.0-1.0 ) is converted into RGB values
	*/
	enum PlotMode {
		/// simple greyscale represantation
		PLOT_GREYSCALE,
		/// color gradient from blue -> cyan -> green -> yellow -> red
		PLOT_HEATMAP
	};

	MapPlotter();

	/**
		Set the data to convert.
		@param data an 2d-Array of floats, stored linearised.
		@param Size the size of the 2d array given in data in x/y
		@param normalize if set to false the data is assumed to be normalized ( valures range from 0.0 to 1.0). Otherwise the lowest value will be scaled to 0, the highest to 1
	*/
	void setData( std::vector<float>& data, Geom::Vec2 _Size, bool normalize = false );

	/**
		Define which plot mode to use.
		@see PlotMode
	*/
	void setMode( PlotMode m ) { Mode = m; }

	/// return the current PlotMode
	PlotMode getMode( ) { return Mode; }

	/**
		Draw the current data to the internal Image. Result may be accessed via getImage().
	*/
	void plot( );

	/// return the rendered Image
	sf::Image& getImage() { return Img; }

private:

	/// our input data
	std::vector<float> Data;
	/// the size of our input data (x/y)
	Geom::Vec2 Size;

	/// which plotmode to use
	PlotMode Mode;

	/// converts a float value to rgb-colors, according to the plotting Mode
	sf::Color getColor( float f);
	/// this holds the rendered image
	sf::Image Img;

};

#endif // MAPPLOT_H

