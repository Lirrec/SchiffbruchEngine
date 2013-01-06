#ifndef IOPLUGIN_H
#define IOPLUGIN_H



#include <vector>
#include <string>

#include <istream>
#include <ostream>

//namespace std
//{
//	class istream;
//	class ostream;
//}

#include <boost/property_tree/ptree.hpp>

//namespace boost
//{
//	namespace property_tree
//	{
//		class ptree;
//	}
//}


/**
	Generic IO Plugin. Base Class for Binary and Tree Plugins
*/
class IOPlugin
{

public:

	enum class loader {
		BINARY,
		PTREE
	} loader_type;

	/**
		This constructor is the only one provided to make sure loader_type is always set.
	*/
	IOPlugin ( loader ldtype, const std::string& relpath )
	: loader_type(ldtype), relative_path(relpath)
	{}

	virtual ~IOPlugin() {};

	/**
		@return a vector of string containing the supported file endinges (e.g. "png", "jpg", "jpeg")
	*/
	virtual const std::vector<std::string>& getSupportedFileExtensions() = 0;

	/**
			Defines the relative path (binary loader) or file(ptree loader) which is used to store all Resources of this type.
			Example: the "textures" subdirectory for all textures
			Example: the "Creatures.info" file for all creatures
	*/
	std::string relative_path;



};


template<class T>
class iBinaryIOPlugin : public IOPlugin
{

	public:

		iBinaryIOPlugin( const std::string& relpath )
		: IOPlugin ( loader::BINARY, relpath )
		{}

		virtual ~iBinaryIOPlugin() {};

        typedef std::vector<std::shared_ptr<T>> ObjectList;

        virtual ObjectList decodeStream(std::istream& in) = 0;

        /**
        * @return true on successfull encoding
        */
        virtual bool encodeStream( const T& object, std::ostream& out) = 0;

		/**
			@return a vector of string containing the supported file endinges (e.g. "png", "jpg", "jpeg")
		*/
		virtual const std::vector<std::string>& getSupportedFileExtensions() = 0;

};

template<class T>
class iTreeIOPlugin : public IOPlugin
{

	public:

		iTreeIOPlugin( const std::string& relpath )
		: IOPlugin ( loader::PTREE, relpath )
		{}

        typedef std::shared_ptr<T> ObjPtr;

        virtual ObjPtr loadObject(const boost::property_tree::ptree::value_type& node) = 0;

        /**
        * @return true on successfull encoding
        */
        virtual bool saveObject( const std::string& name, const T& object, boost::property_tree::ptree& root) = 0;

        virtual const std::vector<std::string>& getSupportedFileExtensions() {
        	 static std::vector<std::string> fileexts { "info" };
        	 return fileexts;
        	 }

};

#endif
