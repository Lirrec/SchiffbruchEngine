#ifndef ImageIOPlugin_HPP
#define ImageIOPlugin_HPP

#include <SFML/Graphics/Image.hpp>

#include <istream>
#include <ostream>

#include "sbe/io/IOPlugin.hpp"

namespace sbe
{

	/// iBinaryIOPlugin for sf::Image
	class ImageIOPlugin : public iBinaryIOPlugin<sf::Image>
	{
	public:
		ImageIOPlugin();

		virtual ~ImageIOPlugin();

		virtual ObjectList decodeStream(std::istream &in);
		virtual bool encodeStream( const sf::Image &o, std::ostream &out);
		virtual const std::vector<std::string>& getSupportedFileExtensions();
	};

} // namespace sbe
#endif // TERRAINIOPLUGIN_HPP

