/**
	IO Implementation file.
*/

#include <type_index>

template< typename T>
void IO::addBinaryPlugin(std::shared_ptr<iBinaryIOPlugin<T>> IOP)
{
	BinaryPlugins[ type_index(typeof(T)) ] = IOP;
}

template< typename T>
void IO::addTreePlugin(std::shared_ptr<iTreeIOPlugin<T>> IOP)
{
	TreePlugins[ type_index(typeof(T)) ] = IOP;
}

template<class T>
std::shared_ptr<T> loadObjectFile<T>( const std::string& filename )
{

}

template<class T>
bool IO::saveObject<T>( std::shared_ptr<T> pObj, const std::string& path )
{

}

void IO::loadDirectory( const std::string& path )
{

}

bool IO::loadFile( const std::string& path )
{

}

void IO::addPath( const std::string& path )
{

}

