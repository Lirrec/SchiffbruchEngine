#ifndef MAPPLOT_H
#define MAPPLOT_H

#include "sbe/geom/Point.hpp"

#include <vector>
#include <SFML/Graphics/Image.hpp>

namespace sbe
{
	/**

		A class for drawing 2-dimensional maps.
		Used to create visual Heatmaps, Heightmaps Humiditymaps out of a 2d Array

	*/
	class MapPlotter
	{
	public:

		/**
			Available plotting modes.
			These modes define how the normalized input data ( 0.0-1.0 ) is converted into RGB values
			Default: GreyScale
		*/
		enum PlotMode
		{
			/// simple greyscale representation
					PLOT_GREYSCALE,
			/// color gradient from blue -> cyan -> green -> yellow -> red
					PLOT_HEATMAP
		};

		/// default ctor
		MapPlotter();

		MapPlotter(const std::string& _name, PlotMode m = PLOT_GREYSCALE)
				: name(_name), Size(0, 0), Mode(m) { }

		/**
			Set the data to convert.
			@param data an 2d-Array of floats, stored linearised.
			@param _Size the size of the 2d array given in data in x/y
			@param normalize if set to false the data is assumed to be normalized ( valures range from 0.0 to 1.0). Otherwise the lowest value will be scaled to 0, the highest to 1
		*/
		void setData(std::vector<float>& data, glm::uvec2 _Size, bool normalize = false);

		/**
			Define which plot mode to use.
			@see PlotMode
		*/
		void setMode(PlotMode m) { Mode = m; }

		/// return the current PlotMode
		PlotMode getMode() { return Mode; }

		/// set the name
		void setName(const std::string& _name) { name = _name; }

		/// get current name
		std::string& getName() { return name; }

		/**
			Draw the current data to the internal Image.
			Result may be accessed via getImage().
		*/
		void plot();

		/// return the rendered Image
		sf::Image& getImage() { return Img; }

	private:

		/// a string name for this plot
		std::string name;

		/// our input data
		std::vector<float> Data;
		/// the size of our input data (x/y)
		glm::uvec2 Size;

		/// which plotmode to use
		PlotMode Mode;

		/// converts a float value to rgb-colors, according to the plotting Mode
		sf::Color getColor(float f);

		/// this holds the rendered image
		sf::Image Img;

	};
} // namespace sbe
#endif // MAPPLOT_H
