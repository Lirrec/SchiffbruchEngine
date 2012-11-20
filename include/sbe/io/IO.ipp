/**
	IO Implementation file.
*/

#include <typeindex>

template< typename T>
void IO::addBinaryPlugin(std::shared_ptr<iBinaryIOPlugin<T>> IOP)
{
	BinaryPlugins[ type_index(typeid(T)) ] = boost::any(IOP);
}

template< typename T>
void IO::addTreePlugin(std::shared_ptr<iTreeIOPlugin<T>> IOP)
{
	TreePlugins[ type_index(typeid(T)) ] = boost::any(IOP);
}

template<class T>
std::shared_ptr<T> loadObjectFile( const std::string& filename )
{

}

template<class T>
bool IO::saveObject( std::shared_ptr<T> pObj, const std::string& path )
{

}

void IO::loadPath( const std::string& path )
{

}

void IO::addPath( const std::string& path )
{

}

