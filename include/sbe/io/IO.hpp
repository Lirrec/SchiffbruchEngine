#ifndef IO_H
#define IO_H

#include <memory>
#include <typeindex>
#include <typeinfo>

#include <map>
#include <vector>
#include <deque>
#include <iterator>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

#include <fstream>
#include <iterator>

#include "sbe/io/IOPlugin.hpp"

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

namespace sbe
{

	/**
		Manager Class for the IO System.
		For loading and saving classes an appropriate IOPlugin has to be implemented and registered with addPlugin()
		@see IOPlugin.
		The Engine provides IOPlugins for sf::Image, ImageSet, sf::Shader and sf::Sound.

		IO also manages a list of the current working paths ( addPath(), popPath() ).
		Those are used to determine the default saving locations and search directories for loading ( relative paths are always interpreted relative to the top Path ).
		If the top path doesnt contain the target when trying to load a file all other paths are also searched.

		This allows to set a default resource-directory in which all config files and resources are looked up by default.
		Please note that IOPlugins may define a subdirectory or file in which their resources are searched for ( e.g. the ImageIOPlugin searches in the "texture" subdirectory ).
		Also if Resources are stored in a boost::property::tree instead of binary this value does contain a file in which to search for the object, no path.
	*/
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
				Add a path on top of the search path stack.
				Missing resources are searched for in all paths starting from the top.
			*/
			bool addPath( const std::string& path );

			/**
				Removes the top of the search path stack and returns it.
			*/
			std::string popPath( );

			/**
				Returns the top of the search path stack ( without modifing the path stack ).
			*/
			std::string topPath( );


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

			/**
				Tries to load all Objects from the default directory of the given types IOPlugin ( e.g. for sf::Image all Images from the "texture" subdirectory relative to the top path )
			*/
			template <class T>
			std::vector<std::shared_ptr<T> > loadObjects( );

			/**
				Tries to load all Objects from the given file.
				@param T the type of objects to load
				@param name the filename to load the objects from.
				@return a vector of loaded objects, may be empty
			*/
			template <class T>
			std::vector<std::shared_ptr<T> > loadObjects( const std::string name );

			/**
				Tries to save a given object to a file.
				@param name the filename to save the object to
				@param object the object to save
				@param overwrite should the file be overwritten if it already exists
				@return true on success
			*/
			template<class T>
			bool saveObject( const std::string& name, const T& object, bool overwrite = false );

			/**
				Save a list Objects of a type with additional names.
				This is mainly used to save objects managed by the resourcemanager as those are always identified by a name.
				The objects will be written to the IOPlugins default saving location/file.
				@param Objects a map of names and objects.
				@param overwrite should the file be overwritten if it already exists
				@return true on success, false on error(s)
			*/
			template<class T>
			bool saveObjects( std::map<std::string,std::shared_ptr<T>>& Objects, bool overwrite = false );

			/**
				Save a list Objects of a type with additional names.
				This is mainly used to save objects managed by the resourcemanager as those are always identified by a name.
				The objects will be written to the IOPlugins default saving location/file.
				@param start an iterator to the start of the container being saved
				@param end an iterator to the end of the container being saved
				@param overwrite should the file be overwritten if it already exists
				@return true on success, false on error(s)
			*/
			template<class T, class Iter>
			bool saveObjects( Iter start, Iter end, bool overwrite = false );

		private:

			/// returns a plugin for the given type_index or an empty shared_ptr
			std::shared_ptr<IOPlugin> getPlugin( const std::type_index& ti );

			/// returns an ofstream for a given plugin, name+path
			std::pair<std::shared_ptr<fs::ofstream>, std::string> getOfstream( std::shared_ptr<IOPlugin>& IOP, const std::string& name, const std::string& path, bool overwrite );

			/// loads a path, IOP and basepath are already given
			template<class T>
			std::vector<std::shared_ptr<T>> loadPath(  std::shared_ptr<IOPlugin>& IOP, const std::string& current_path, const std::string& filename);

			/// loads a single file
			template<class T>
			std::vector<std::shared_ptr<T>> loadFile( std::shared_ptr<IOPlugin>& IOP, const fs::path& p );

			/// save a single object
			template <class T>
			bool saveFile( std::shared_ptr<IOPlugin>& IOP, const T& object, const std::string& name, fs::ofstream& out);

			std::deque<std::string> Paths;
			std::map<std::type_index, std::shared_ptr<IOPlugin>> Plugins;
	};

} // namespace sbe

#include "../../../src/io/IO.ipp"

#endif // IO_H
