#ifndef IO_H
#define IO_H

#include <memory>
#include <typeindex>
#include <typeinfo>

#include <map>


class IO
{

	public:

        template< typename T>
        void addBinaryPlugin(std::shared_ptr<iBinaryIOPlugin<T>> LP);

        template< typename T>
        void addTreePlugin(std::shared_ptr<iTreeIOPlugin<T>> LP);

        template<class T>
        std::shared_ptr<T> loadObjectFile<T>( const std::string& filename );

        template<class T>
        bool saveObject<T>( std::shared_ptr<T> pObj, const std::string& path );

        void loadDirectory( const std::string& path );

        bool loadFile( const std::string& path );

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

#endif IO_H
