/**
	@file template implementation of IO ( include/IO.hpp )
*/


#include "sbe/Engine.hpp"

template< typename T>
void IO::addPlugin(std::shared_ptr<IOPlugin> IOP)
{
	auto ti = std::type_index( typeid(T) );

	if (Plugins.find( ti ) == Plugins.end())
	{
		Engine::out() << "[IO] Registering Plugin for '" << ti.name() << "'" << std::endl;
		Plugins[ ti ] = IOP;
	}
	else
	{
		Engine::out() << "[IO] Plugin for '" << ti.name() << "' already registered!" << std::endl;
	}
}

template<class T>
std::vector<std::shared_ptr<T>> IO::loadPath( const std::string& filename )
{
	auto ti = std::type_index(typeid(T));

	// decompose path

	try {

		//determine plugin
		if ( Plugins.find( ti ) == Plugins.end() )
		{
			Engine::out() << "[IO] No IOPlugin found for type '" << ti.name() << "'!" << std::endl;
			return std::vector<std::shared_ptr<T>>();
		}

		std::shared_ptr<IOPlugin> IOP = Plugins[ti];

		for ( std::string current_path : Paths )
		{
			fs::path cp(current_path);

			if ( !fs::exists(cp ) )
			{
				Engine::out() << "[IO] Unable to load path! '" << cp.generic_string() << "' not found!" << std::endl;
				return std::vector<std::shared_ptr<T>>();
			}


			// check if we got a valid file
			if ( fs::is_regular_file(cp / filename)  ) return loadFile<T>( IOP , cp / filename );

			cp /= IOP->relative_path;
			cp /= filename;

			if ( fs::is_regular_file(cp)  ) return loadFile<T>( IOP , cp );

			if ( fs::is_directory(cp) )
			{
				if (IOP->loader_type == IOPlugin::loader::PTREE )
				{
					Engine::out() << "[IO] Unable to load directory '" << cp.generic_string() << "' into a ptree!" << std::endl;
					return std::vector<std::shared_ptr<T>>();
				}
				else
				{
					std::vector<std::shared_ptr<T>> re;

					for ( auto it = fs::directory_iterator(cp); it != fs::directory_iterator(); ++it  )
					{
						if ( fs::is_regular_file( *it ) )
						{
							auto tmp = loadFile<T>( IOP , cp );
							re.insert( re.end(), tmp.begin(), tmp.end() );
						}
					}

					return re;
				}
			}
			else
			{
				Engine::out() << "[IO] Unable to load path! '" << cp.generic_string() << "' not a directory nor a file!" << std::endl;
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
std::vector<std::shared_ptr<T>> IO::loadFile( std::shared_ptr<IOPlugin>& IOP, const fs::path& p )
{
	auto ti = std::type_index( typeid(T) );

	if (std::find(IOP->getSupportedFileExtensions().begin(), IOP->getSupportedFileExtensions().end(), p.extension()) == IOP->getSupportedFileExtensions().end() )
	{
		Engine::out() << "[IO] Unsupported file extension '" << p.extension() << "' for '" << ti.name() << "'!" << std::endl;
		return std::vector<std::shared_ptr<T>>();
	}

	fs::ifstream in(p);

	if (IOP->loader_type == IOPlugin::loader::BINARY )
	{
			Engine::out() << "[IO] Loaded " << p << " ( " << ti.name() << ")" << std::endl;
			auto BinIO = dynamic_pointer_cast<iBinaryIOPlugin<T>>(IOP);

			return BinIO->decodeStream ( in );
	}
	else
	{
		auto TreeIO = dynamic_pointer_cast<iTreeIOPlugin<T>>(IOP);

		std::vector<std::shared_ptr<T>> re;

		pt::ptree tree;
		pt::info_parser::read_info( in, tree );

		for (pt::ptree::value_type& e : tree)
		{
			std::shared_ptr<T> ptr = TreeIO->loadObject( e );
			if (ptr) re.push_back(ptr);
		}

		Engine::out() << "[IO] Loaded " << p << " ( " << re.size() << " " << ti.name() << "s)" << std::endl;

		return re;
	}
}
