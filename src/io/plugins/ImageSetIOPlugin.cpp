#include "ImageSetIOPlugin.hpp"

using boost::property_tree::ptree;

ImageSetIOPlugin()
{
	file = "ImageSets.info";
}

~ImageSetIOPlugin()
{

}


ObjectList loadObjects(boost::property_tree::ptree& root)
{
	ObjectList re;


}


bool saveObject( const ImageSet& object, boost::property_tree::ptree& root)
{
	ptree pt;

	pt.put("Name", object.Name);
	pt.put("ImageName", object.ImageName);
	pt.put("StartPos.x", object.StartPos.x());
	pt.put("StartPos.y", object.StartPos.y());

	pt.put("DestPos.x", object.DestPos.x());
	pt.put("DestPos.y", object.DestPos.y());

	pt.put("FrameSize.x", object.FrameSize.x());
	pt.put("FrameSize.y", object.FrameSize.y());

	pt.put("FrameCount.x", object.FrameCount.x());
	pt.put("FrameCount.y", object.FrameCount.y());

	pt.put("FramesPerSecond", object.FramesPerSecond);
	pt.put("NumFrames", object.NumFrames);

	root.add("ImageSets.ImageSet", pt);
}
