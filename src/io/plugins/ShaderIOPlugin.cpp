#include "ShaderIOPlugin.hpp"

#include <sbe/Engine.hpp>

using boost::property_tree::ptree;

ShaderIOPlugin::~ShaderIOPlugin()
{

}

ShaderIOPlugin::ObjPtr ShaderIOPlugin::loadObject(const boost::property_tree::ptree::value_type &node){
		ObjPtr re;
	if (node.first != "Shader") return re;

	try{
		const ptree& pt = node.second;

		string name = pt.get<string>("name");
		string base = Engine::getIO()->topPath() + "/shader/";

		re.reset( new sf::Shader() );

		re->loadFromFile(base + pt.get<string>("vert") + ".vert",
		                 base + pt.get<string>("frag") + ".frag")


	} catch( boost::property_tree::ptree_error )
	{
		Engine::out() << "[ShaderIOPlugin] Error loading Shader from ptree!";
		re.reset();
		return re;
	}

	return re;
}


bool saveObject( const std::string& name, const sf::Shader &o, boost::property_tree::ptree &root){
	Engine::out(Engine::ERROR) << "[ShaderIOPlugin] Saving shaders not possible!";
}
