#ifndef IO_H
#define IO_H

#include <memory>
#include <typeindex>
#include <typeinfo>

#include <map>
#include "sbe/io/IOPlugin.hpp"

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
			If there was already an IOPlugin registerd the new one is used from now on.
		*/
        template< typename T>
        void addTreePlugin(std::shared_ptr<iTreeIOPlugin<T>> IOP);



		/**
			Tries to load a given file.
			The used plugin will be determined by the supported file extensions of the registered plugins
		*/
        template<class T>
        std::vector<std::shared_ptr<T>> loadObjectFile( const std::string& filename );

        template<class T>
        bool saveObject( std::shared_ptr<T> pObj, const std::string& path );

	    void loadPath(  const std::string& path );

        void addPath( const std::string& path );

    private:

        /**
        Register IOPlugins for all the Classes supported by default by the engine.
        This includes:
            sf::Texture
            sf::Soundbuffer
            sf::Music
            sf::Font
            ImageSet

        */
        void loadDefaultPlugins();

        std::map<std::type_index, boost::any> BinaryPlugins; // boost::any contains iBinaryIOPlugin<T>
        std::map<std::type_index, boost::any> TreePlugins;   // boost::any contains iTreeIOPlugin<T>
};

#include "IO.ipp"

#endif // IO_H
