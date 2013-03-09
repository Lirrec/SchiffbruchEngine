#ifndef SHADERIOPLUGIN_HPP
#define SHADERIOPLUGIN_HPP




#include <boost/property_tree/ptree.hpp>

#include <sbe/io/IOPlugin.hpp>
#include <SFML/Graphics/Shader.hpp>

/// iTreeIOPlugin for sf::Shader
class ShaderIOPlugin : public iTreeIOPlugin<sf::Shader> {
public:
	ShaderIOPlugin()
	 : iTreeIOPlugin( "Shaders.info" )
	{}

	virtual ~ShaderIOPlugin();
	virtual ObjPtr loadObject(const boost::property_tree::ptree::value_type &node);
	virtual bool saveObject( const std::string& name, const sf::Shader &o, boost::property_tree::ptree &root);

};


#endif // SHADERIOPLUGIN_HPP
