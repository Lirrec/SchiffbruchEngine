#ifndef IO_H
#define IO_H

#include <memory>
#include <typeindex>
#include <typeinfo>

#include <map>
#include "sbe/io/IOPlugin.hpp"

using namespace std;

class IO
{

	public:

		/**
			Registers an IOPlugin for a given Type.
			If there was already an IOPlugin registerd the new one is used from now on.
		*/
        template< typename T>
        void addBinaryPlugin(std::shared_ptr<iBinaryIOPlugin<T>> IOP);

		/**
			Registers an IOPlugin for a given Type.
			If there was already an IOPlugin registered the new one is used from now on.
		*/
        template< typename T>
        void addTreePlugin(std::shared_ptr<iTreeIOPlugin<T>> IOP);


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

		std::stack<std::string,std::deque> Paths;

        std::map<std::type_index, boost::any> BinaryPlugins; // boost::any contains iBinaryIOPlugin<T>
        std::map<std::type_index, boost::any> TreePlugins;   // boost::any contains iTreeIOPlugin<T>
};

#include "../../../src/io/IO.ipp"

#endif // IO_H
