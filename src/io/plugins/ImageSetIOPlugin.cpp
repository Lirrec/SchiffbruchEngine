#include "ImageSetIOPlugin.hpp"

namespace sbe
{
	using boost::property_tree::ptree;

	ImageSetIOPlugin::ImageSetIOPlugin()
			: iTreeIOPlugin("ImageSets.info") { }

	ImageSetIOPlugin::~ImageSetIOPlugin() {

	}


	ImageSetIOPlugin::ObjPtr ImageSetIOPlugin::loadObject(const boost::property_tree::ptree::value_type& node) {
		ObjPtr re;
		if (node.first != "ImageSet") return re;

		try
		{
			const ptree& pt = node.second;

			re = std::make_shared<ImageSet>();

			re->Name = pt.get<std::string>("Name");
			re->ImageName = pt.get<std::string>("ImageName");
			re->StartPos.x = pt.get<int>("StartPos.x");
			re->StartPos.y = pt.get<int>("StartPos.y");

			re->DestPos.x = pt.get<int>("DestPos.x");
			re->DestPos.y = pt.get<int>("DestPos.y");

			re->FrameSize.x = pt.get<int>("FrameSize.x");
			re->FrameSize.y = pt.get<int>("FrameSize.y");

			re->FrameCount.x = pt.get<int>("FrameCount.x");
			re->FrameCount.y = pt.get<int>("FrameCount.y");

			re->FramesPerSecond = pt.get < unsigned
			int > ("FramesPerSecond");

			re->calcNumFrames();

		}
		catch (boost::property_tree::ptree_error)
		{
			Engine::out(Engine::ERROR) << "[ImageSetIOPlugin] Error loading ImageSet from ptree!" << std::endl;
			re.reset();
			return re;
		}

		return re;
	}


	bool ImageSetIOPlugin::saveObject(const std::string& name, const ImageSet& object, boost::property_tree::ptree& root) {
		try
		{
			ptree pt;

			pt.put("Name", name);
			pt.put("ImageName", object.ImageName);
			pt.put("StartPos.x", object.StartPos.x);
			pt.put("StartPos.y", object.StartPos.y);

			pt.put("DestPos.x", object.DestPos.x);
			pt.put("DestPos.y", object.DestPos.y);

			pt.put("FrameSize.x", object.FrameSize.x);
			pt.put("FrameSize.y", object.FrameSize.y);

			pt.put("FrameCount.x", object.FrameCount.x);
			pt.put("FrameCount.y", object.FrameCount.y);

			pt.put("FramesPerSecond", object.FramesPerSecond);
			//pt.put("NumFrames", object.NumFrames);

			root.add_child("ImageSet", pt);
			return true;
		}
		catch (boost::property_tree::ptree_error)
		{
			Engine::out(Engine::ERROR) << "[ImageSetIOPlugin] Error saving ImageSet to ptree!" << std::endl;
			return false;
		}
	}
} // namespace sbe
