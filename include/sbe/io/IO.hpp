#ifndef IO_H
#define IO_H

#include <memory>
#include <typeindex>
#include <typeinfo>

#include <map>
#include <vector>
#include <deque>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

#include <fstream>

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

#include "sbe/io/IOPlugin.hpp"

using namespace std;

class IO
{

	public:
		IO();
		~IO();


		/**
			Registers an IOPlugin for a given Type.
			If there was already an IOPlugin registerd the new one is used from now on.
		*/
        template< typename T>
        void addPlugin(std::shared_ptr<IOPlugin> IOP);



		/**
			Add a path on top of the search path stack
			Missing resources are searched for in all paths starting from the top
		*/
        void addPath( const std::string& path );

		/**
			Removes the top of the search path stack and returns it
		*/
		std::string popPath( );


		/**
			Tries to load a given path.
			The used plugin will be determined by the template parameter.
			If the given path is a directory all files in the directory of the given type will be loaded.
			This function is used by the Resourcemanager to retrieve loaded resources.
			@param filename ( for ptrees or single resources (e.g. textures) ) or path ( for binary resources ) to load. It is searched for in all paths registered via addPath()
			@return zero or more resources depending on the given file ( usually 1 element for textures etc, but possibly more if the file is a ptree containing a list of items )
		*/
        template<class T>
        std::vector<std::shared_ptr<T>> loadPath( const std::string& filename );

    private:

		template<class T>
		std::vector<std::shared_ptr<T>> loadFile( std::shared_ptr<IOPlugin>& IOP, const fs::path& p );

		std::deque<std::string> Paths;
        std::map<std::type_index, std::shared_ptr<IOPlugin>> Plugins;
};

#include "../../../src/io/IO.ipp"

#endif // IO_H
