/**
	@file template implementation of IO ( include/IO.hpp )
*/


#include "sbe/Engine.hpp"



template< typename T>
void IO::addBinaryPlugin(std::shared_ptr<iBinaryIOPlugin<T>> IOP)
{
	auto ti = std::type_index( typeid(T) );

	if (BinaryPlugins.find( ti ) == BinaryPlugins.end())
	{
		Engine::out() << "[IO] Registering BinaryPlugin for '" << ti.name() << "'" << std::endl;
		BinaryPlugins[ ti ] = IOP;
	}
	else
	{
		Engine::out() << "[IO] BinaryPlugin for '" << ti.name() << "' already registered!" << std::endl;
	}
}


template< typename T>
void IO::addTreePlugin(std::shared_ptr<iTreeIOPlugin<T>> IOP)
{
	auto ti = std::type_index( typeid(T) );

	if (TreePlugins.find( ti ) == TreePlugins.end())
	{
		Engine::out() << "[IO] Registering TreePlugin for '" << ti.name() << "'" << std::endl;
		TreePlugins[ ti ] = IOP;
	}
	else
	{
		Engine::out() << "[IO] TreePlugin for '" << ti.name() << "' already registered!" << std::endl;
	}
}


template<class T>
std::vector<std::shared_ptr<T>> IO::loadPath( const std::string& filename )
{
	auto ti = std::type_index(typeid(T));

	// decompose path

	try {

		for ( std::string current_path : Paths )
		{
			fs::path cp(current_path);
			cp /= filename;

			if ( fs::exists(cp ) )
			{
				if ( fs::is_regular_file(cp)  )
				{
					if ( BinaryPlugins.find( ti ) != BinaryPlugins.end() ) return loadFileBinary<T>( BinaryPlugins[ti], cp );
					else if (TreePlugins.find( ti ) != TreePlugins.end() ) return loadFileTree<T>( TreePlugins[ti], cp );
				}
				else if ( fs::is_directory(cp) )
				{
					for ( auto it = fs::directory_iterator(cp); it != fs::directory_iterator(); ++it  )
					{
						if ( fs::is_regular_file( *it ) )
						{
							if ( BinaryPlugins.find( ti ) != BinaryPlugins.end() ) return loadFileBinary<T>( BinaryPlugins[ti], cp );
							else if (TreePlugins.find( ti ) != TreePlugins.end() ) return loadFileTree<T>( TreePlugins[ti], cp );
						}
					}
				}
				else
				{
					Engine::out() << "[IO] Unable to load path! '" << cp.generic_string() << "' not a directory nor a file!" << std::endl;
				}
			}
			else
			{
				Engine::out() << "[IO] Unable to load path! '" << cp.generic_string() << "' not found!" << std::endl;
			}
		}

	}
	catch (fs::filesystem_error& e)
	{
		Engine::out() << "[IO] boost::fs exception! '" << e.what() << "'" << std::endl;
	}

	return std::vector<std::shared_ptr<T>>();
}

template<class T>
std::vector<std::shared_ptr<T>> IO::loadFileBinary( const iBinaryIOPlugin<T>& IOP, const fs::path& p )
{
	auto ti = std::type_index( typeid(T) );

	if (IOP.getSupportedFileExtensions().find( p.extension() ) != IOP.getSupportedFileExtensions().end())
	{
		return IOP.decodeStream ( fs::ifstream(p) );
		Engine::out() << "[IO] Loaded " << p << " ( " << ti.name() << ")" << std::endl;
	}
	else
	{
		Engine::out() << "[IO] Unsupported file extension '" << p.extension() << "' for '" << ti.name() << "'!" << std::endl;
		return std::vector<std::shared_ptr<T>>();
	}
}

template<class T>
std::vector<std::shared_ptr<T>> IO::loadFileTree( const iTreeIOPlugin<T>& IOP, const fs::path& p )
{
	auto ti = std::type_index( typeid(T) );

	std::vector<std::shared_ptr<T>> re;

	if (IOP.getSupportedFileExtensions().find( p.extension() ) != IOP.getSupportedFileExtensions().end())
	{
		fs::ifstream in(p);

		pt::ptree tree;
		pt::info_parser::read_info( in, tree );

		for (pt::ptree::value_type& e : tree)
		{
			std::shared_ptr<T> ptr = IOP.loadObject( e );
			if (ptr) re.push_back(ptr);
		}

		Engine::out() << "[IO] Loaded " << p << " ( " << re.size() << " " << ti.name() << "s)" << std::endl;

	}
	else
	{
		Engine::out() << "[IO] Unsupported file extension '" << p.extension() << "' for '" << ti.name() << "'!" << std::endl;
	}

	return re;
}



