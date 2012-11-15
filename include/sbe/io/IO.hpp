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
        std::vector<std::shared_ptr<T>> loadObjectFile<T>( const std::string& filename );

        template<class T>
        bool saveObject<T>( std::shared_ptr<T> pObj, const std::string& path );

	    void loadPath(  )

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

        std::map<std::type_index, iBinaryIOPlugin> BinaryPlugins;
        std::map<std::type_index, iTreeIOPlugin> TreePlugins;
}

#include "IO.ipp"

#endif IO_H
