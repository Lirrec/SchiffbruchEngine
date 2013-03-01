#include "ShaderIOPlugin.hpp"

#include <sbe/Engine.hpp>
#include <sbe/io/IO.hpp>

using boost::property_tree::ptree;

ShaderIOPlugin::~ShaderIOPlugin()
{

}

ShaderIOPlugin::ObjPtr ShaderIOPlugin::loadObject(const boost::property_tree::ptree::value_type &node){
		ObjPtr re;
	if (node.first != "Shader") return re;

	try{
		const ptree& pt = node.second;

		std::string name = pt.get<std::string>("name");
		std::string base = Engine::GetIO()->topPath() + "/shader/";

		re.reset( new sf::Shader() );

		re->loadFromFile(base + pt.get<std::string>("vert") + ".vert",
		                 base + pt.get<std::string>("frag") + ".frag");


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
