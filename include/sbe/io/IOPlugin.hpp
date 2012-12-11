#ifndef IOPLUGIN_H
#define IOPLUGIN_H

#include <boost/property_tree/ptree.hpp>

#include <vector>
#include <string>

template<class T>
class iBinaryIOPlugin // : IOPlugin
{

	public:
        typedef std::vector<std::shared_ptr<T>> ObjectList;


        virtual ObjectList decodeStream(std::istream& in) = 0;


        /**
        * @return true on successfull encoding
        */
        virtual bool encodeStream( const T& object, std::ostream& out) = 0;

		/**
			@return a vector of string containing the supported file endinges (e.g. "png", "jpg", "jpeg")
		*/
		virtual std::vector<std::string> getSupportedFileExtensions() = 0;

		/**
			Defines the relative path which is used to store all Resources of this type.
			Example: the "textures" subdirectory for all textures
		*/
		std::string path;

};

template<class T>
class iTreeIOPlugin // : IOPlugin
{

	public:
        typedef std::vector<std::shared_ptr<T>> ObjectList;

        virtual ObjectList loadObjects(boost::property_tree::ptree& root) = 0;

        /**
        * @return true on successfull encoding
        */
        virtual bool saveObject( const T& object, boost::property_tree::ptree& root) = 0;

        std::vector<std::string> getSupportedFileExtensions() { return { "info" }; }

		/**
			Defines the relative file which is used to store all Resources of this type.
			Example: the "Creatures.info" file for all creatures
		*/
		std::string file;
};

#endif
