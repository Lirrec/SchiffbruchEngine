#ifndef IMAGESETIOPLUGIN_HPP
#define IMAGESETIOPLUGIN_HPP

#include "sbe/gfx/ImageSet.hpp"

#include "sbe/io/IOPlugin.hpp"

#include <boost/property_tree/ptree.hpp>

namespace sbe
{

	/// iTreeIOPlugin for ImageSet
	class ImageSetIOPlugin : public iTreeIOPlugin<ImageSet>
	{
	public:
		ImageSetIOPlugin();

		virtual ~ImageSetIOPlugin();

		virtual ObjPtr loadObject(const boost::property_tree::ptree::value_type& node);

		virtual bool saveObject(const std::string& name, const ImageSet& object, boost::property_tree::ptree& root);
	};

} // namespace sbe
#endif // IMAGESETIOPLUGIN_HPP
