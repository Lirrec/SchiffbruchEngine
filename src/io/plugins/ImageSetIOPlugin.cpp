#include "ImageSetIOPlugin.hpp"

using boost::property_tree::ptree;

ImageSetIOPlugin::ImageSetIOPlugin()
{
	file = "ImageSets.info";
}

ImageSetIOPlugin::~ImageSetIOPlugin()
{

}


ImageSetIOPlugin::ObjPtr ImageSetIOPlugin::loadObject(const boost::property_tree::ptree::value_type& node)
{
	ObjPtr re;
	if (node.first != "ImageSet") return re;

	try
	{
		const ptree& pt = node.second;

		re.reset(new ImageSet());

		re->Name = pt.get<std::string>("Name");
		re->ImageName = pt.get<std::string>("ImageName");
		re->StartPos.SetX( pt.get<int>("StartPos.x"));
		re->StartPos.SetY( pt.get<int>("StartPos.y"));

		re->DestPos.SetX( pt.get<int>("DestPos.x"));
		re->DestPos.SetY( pt.get<int>("DestPos.y"));

		re->FrameSize.SetX( pt.get<int>("FrameSize.x"));
		re->FrameSize.SetY( pt.get<int>("FrameSize.y"));

		re->FrameCount.SetX( pt.get<int>("FrameCount.x"));
		re->FrameCount.SetY( pt.get<int>("FrameCount.y"));
	}
	catch ( boost::property_tree::ptree_error )
	{
		Engine::out() << "[ImageSetIOPlugin] Error loading ImageSet from ptree!" << std::endl;
		re.reset();
		return re;
	}

	return re;
}


bool ImageSetIOPlugin::saveObject( const ImageSet& object, boost::property_tree::ptree& root)
{
	try {
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

		root.add("ImageSet", pt);
		return true;
	}
	catch ( boost::property_tree::ptree_error )
	{
		Engine::out() << "[ImageSetIOPlugin] Error saving ImageSet to ptree!" << std::endl;
		return false;
	}
}
