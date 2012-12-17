/**
	@file template implementation of IO ( include/IO.hpp )
*/

#include "sbe/io/IOPlugin.hpp"

template< typename T>
void IO::addBinaryPlugin(std::shared_ptr<iBinaryIOPlugin<T>> IOP)
{
	if (BinaryPlugins.find( IOP ) == BinaryPlugins.end())
	{
		Engine::out() << "[IO] Registering BinaryPlugin for '" << typeid( T ).name() << "'"
		BinaryPlugins[ std::type_index( typeid( T ) )] = IOP;
	}
	else
	{
		Engine::out() << "[IO] BinaryPlugin for '" << typeid( T ).name() << "' already registered!"
	}
}


template< typename T>
void IO::addTreePlugin(std::shared_ptr<iTreeIOPlugin<T>> IOP)
{
	if (TreePlugins.find( IOP ) == TreePlugins.end())
	{
		Engine::out() << "[IO] Registering TreePlugin for '" << typeid( T ).name() << "'"
		TreePlugins[ std::type_index( typeid( T ) )] = IOP;
	}
	else
	{
		Engine::out() << "[IO] TreePlugin for '" << typeid( T ).name() << "' already registered!"
	}
}


void IO::loadPath(  const std::string& path )
{

}

template<class T>
std::vector<std::shared_ptr<T>> IO::loadPath( const std::string& filename );

void IO::addPath( const std::string& path )
{
	Paths.push ( path );
}


std::string IO::popPath( )
{
	std::string re = Paths.top();
	Paths.pop();
	return re;
}



