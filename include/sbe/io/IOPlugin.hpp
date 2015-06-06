#ifndef IOPLUGIN_H
#define IOPLUGIN_H

#include <vector>
#include <string>

#include <istream>
#include <ostream>

#include <boost/property_tree/ptree.hpp>

namespace sbe
{
	/**
		Generic IO Plugin.
		Base Class for Binary and Tree Plugins.
		Currently two types of Plugins are supported:
		- Binary: Objects are saved in a custom way to a binary outputstream
		- Tree: Objects are saved to a boost property tree


	*/
	class IOPlugin
	{

	public:

		enum class loader
		{
			BINARY,
			PTREE
		} loader_type;

		/**
			This constructor is the only one provided to make sure loader_type is always set.
		*/
		IOPlugin(loader ldtype, const std::string& relpath)
				: loader_type(ldtype), relative_path(relpath) { }

		virtual ~IOPlugin() { };

		/**
			Returns supported file extensions by the plugin.
			@return a vector of string containing the supported file endinges (e.g. "png", "jpg", "jpeg")
		*/
		virtual const std::vector<std::string>& getSupportedFileExtensions() = 0;

		/**
				Defines the relative path (binary loader) or file(ptree loader) which is used to store all Resources of this type.
				Example ( binary plugin ) : the "textures" subdirectory for all textures
				Example ( ptree plugin  ): the "Creatures.info" file for all creatures
		*/
		std::string relative_path;

	};

	/**
		A Binary IOPlugin.
		This plugin saves its objects to an outputstream.
	*/
	template<class T>
	class iBinaryIOPlugin : public IOPlugin
	{

	public:

		iBinaryIOPlugin(const std::string& relpath)
				: IOPlugin(loader::BINARY, relpath) { }

		virtual ~iBinaryIOPlugin() { };

		typedef std::vector<std::shared_ptr<T>> ObjectList;

		/**
			Called when an object should be loaded from a stream.
			@param in the inputstream from which the object should be loaded
			@return a possibly empty ObjectList ( vector of objects )
		*/
		virtual ObjectList decodeStream(std::istream& in) = 0;

		/**
			Called when an object should be saved to a stream.
			@param object the object to be saved
			@param out the outputstream to which the object should be saved
			@return true on successfull encoding
		*/
		virtual bool encodeStream(const T& object, std::ostream& out) = 0;

		/**
			@return a vector of string containing the supported file endinges (e.g. "png", "jpg", "jpeg")
		*/
		virtual const std::vector<std::string>& getSupportedFileExtensions() = 0;

	};

	/**
		A Tree IOPlugin.
		This plugin saves its objects to a boost::ptree.

		Please see their documentation on how to work with boost::property_tree:
		http://www.boost.org/doc/libs/release/libs/property_tree/

	*/
	template<class T>
	class iTreeIOPlugin : public IOPlugin
	{

	public:

		iTreeIOPlugin(const std::string& relpath)
				: IOPlugin(loader::PTREE, relpath) { }

		typedef std::shared_ptr<T> ObjPtr;

		/**
			Called when an object should be loaded.
			@param node the root of the tree which contains the object
			@return a possibly empty shared_ptr to the loaded object
		*/
		virtual ObjPtr loadObject(const boost::property_tree::ptree::value_type& node) = 0;

		/**
			Called when an object should be saved.
			@param name the name of the object
			@param object the object itself
			@param root the root of the ptree to save to
			@return true on successfull encoding
		*/
		virtual bool saveObject(const std::string& name, const T& object, boost::property_tree::ptree& root) = 0;

		virtual const std::vector<std::string>& getSupportedFileExtensions() {
			static std::vector<std::string> fileexts{"info"};
			return fileexts;
		}

	};
} // namespace sbe
#endif
