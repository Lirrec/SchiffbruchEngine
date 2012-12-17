#ifndef IMAGESETIOPLUGIN_HPP
#define IMAGESETIOPLUGIN_HPP

#include "sbe/ImageSet.hpp"

#include "sbe/io/IOPlugin.hpp"

#include <boost/property_tree/ptree.hpp>

class ImageSetIOPlugin : iTreeIOPlugin<ImageSet>
{

	ImageSetIOPlugin();

	virtual ~ImageSetIOPlugin();

	virtual ObjPtr loadObject(const boost::property_tree::ptree::value_type& node);
	virtual bool saveObject( const ImageSet& object, boost::property_tree::ptree& root);
};

#endif // IMAGESETIOPLUGIN_HPP
