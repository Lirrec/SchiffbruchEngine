#include <sbe/util/FactoriesAndManipulators.hpp>

namespace sbe
{
	static std::default_random_engine rng;
	std::default_random_engine& getRNG() { return rng; };

	namespace factories
	{

	}

	namespace interpolate
	{

		struct HsvColor
		{
			unsigned char h;
			unsigned char s;
			unsigned char v;
		};

		sf::Color HsvToRgb(HsvColor hsv)
		{
			sf::Color rgb;
			unsigned char region, p, q, t;
			unsigned int h, s, v, remainder;

			if (hsv.s == 0)
			{
				rgb.r = hsv.v;
				rgb.g = hsv.v;
				rgb.b = hsv.v;
				return rgb;
			}

			// converting to 16 bit to prevent overflow
			h = hsv.h;
			s = hsv.s;
			v = hsv.v;

			region = h / 43;
			remainder = (h - (region * 43)) * 6;

			p = (v * (255 - s)) >> 8;
			q = (v * (255 - ((s * remainder) >> 8))) >> 8;
			t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

			switch (region)
			{
				case 0:
					rgb.r = v;
					rgb.g = t;
					rgb.b = p;
					break;
				case 1:
					rgb.r = q;
					rgb.g = v;
					rgb.b = p;
					break;
				case 2:
					rgb.r = p;
					rgb.g = v;
					rgb.b = t;
					break;
				case 3:
					rgb.r = p;
					rgb.g = q;
					rgb.b = v;
					break;
				case 4:
					rgb.r = t;
					rgb.g = p;
					rgb.b = v;
					break;
				default:
					rgb.r = v;
					rgb.g = p;
					rgb.b = q;
					break;
			}

			return rgb;
		}

		HsvColor RgbToHsv(sf::Color rgb)
		{
			HsvColor hsv;
			unsigned char rgbMin, rgbMax;

			rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
			rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

			hsv.v = rgbMax;
			if (hsv.v == 0)
			{
				hsv.h = 0;
				hsv.s = 0;
				return hsv;
			}

			hsv.s = 255 * ((long)(rgbMax - rgbMin)) / hsv.v;
			if (hsv.s == 0)
			{
				hsv.h = 0;
				return hsv;
			}

			if (rgbMax == rgb.r)
				hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
			else if (rgbMax == rgb.g)
				hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
			else
				hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

			return hsv;
		}

		sf::Color interpolateColor(const sf::Color from, const sf::Color to, const float percentage)
		{
			HsvColor fromhsv = RgbToHsv(from);
			HsvColor tohsv = RgbToHsv(to);

			HsvColor hsv;
			hsv.h = interpolateFloat( fromhsv.h, tohsv.h, percentage );
			hsv.s = interpolateFloat( fromhsv.s, tohsv.s, percentage );
			hsv.v = interpolateFloat( fromhsv.v, tohsv.v, percentage );

			return HsvToRgb(hsv);
		}

	}
}
