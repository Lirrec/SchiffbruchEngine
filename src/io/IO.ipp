/**
	@file template implementation of IO ( include/IO.hpp )
*/


IO::IO()
{
	Paths.push_back(fs::current_path().generic_string());
}

IO::~IO()
{

}




template< typename T>
void IO::addBinaryPlugin(std::shared_ptr<iBinaryIOPlugin<T>> IOP)
{
	if (BinaryPlugins.find( IOP ) == BinaryPlugins.end())
	{
		Engine::out() << "[IO] Registering BinaryPlugin for '" << typeid( T ).name() << "'" << std::endl;
		BinaryPlugins[ std::type_index( typeid( T ) )] = IOP;
	}
	else
	{
		Engine::out() << "[IO] BinaryPlugin for '" << typeid( T ).name() << "' already registered!" << std::endl;
	}
}


template< typename T>
void IO::addTreePlugin(std::shared_ptr<iTreeIOPlugin<T>> IOP)
{
	if (TreePlugins.find( IOP ) == TreePlugins.end())
	{
		Engine::out() << "[IO] Registering TreePlugin for '" << typeid( T ).name() << "'" << std::endl;
		TreePlugins[ std::type_index( typeid( T ) )] = IOP;
	}
	else
	{
		Engine::out() << "[IO] TreePlugin for '" << typeid( T ).name() << "' already registered!" << std::endl;
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
			fs::path cp(current_path + "/" + filename);

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
	if (IOP.getSupportedFileExtensions().find( p.extension() ) != IOP.getSupportedFileExtensions().end())
	{
		return IOP.decodeStream ( fs::ifstream(p) );
		Engine::out() << "[IO] Loaded " << p << " ( " << typeid(T).name() << ")" << std::endl;
	}
	else
	{
		Engine::out() << "[IO] Unsupported file extension '" << p.extension() << "' for '" << typeid(T).name() << "'!" << std::endl;
		return std::vector<std::shared_ptr<T>>();
	}
}

template<class T>
std::vector<std::shared_ptr<T>> IO::loadFileTree( const iTreeIOPlugin<T>& IOP, const fs::path& p )
{
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

		Engine::out() << "[IO] Loaded " << p << " ( " << re.size() << " " << typeid(T).name() << "s)" << std::endl;

	}
	else
	{
		Engine::out() << "[IO] Unsupported file extension '" << p.extension() << "' for '" << typeid(T).name() << "'!" << std::endl;
	}

	return re;
}

void IO::addPath( const std::string& path )
{
	Paths.push_front( path );
}


std::string IO::popPath( )
{
	std::string re = Paths.front();
	Paths.pop_front();
	return re;
}



