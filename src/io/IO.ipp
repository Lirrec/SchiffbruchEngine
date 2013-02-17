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
		//Engine::out() << "[IO] Registering Plugin for '" << IOP.name() << "'" << std::endl;
		//Engine::out() << "[IO] IOPlugin count '" << Plugins.size() << "'" << std::endl;
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
	try {

		std::shared_ptr<IOPlugin> IOP = getPlugin( std::type_index(typeid(T)) );
		if ( !IOP ) return std::vector<std::shared_ptr<T>>();

		for ( const std::string& current_path : Paths )
		{
			return loadPath<T>( IOP, current_path, filename);  // bp: hä?!?
		}

	}
	catch (fs::filesystem_error& e)
	{
		Engine::out(Engine::ERROR) << "[IO] boost::fs exception! '" << e.what() << "'" << std::endl;
	}

	return std::vector<std::shared_ptr<T>>();
}

template<class T>
std::vector<std::shared_ptr<T>> IO::loadPath(  std::shared_ptr<IOPlugin>& IOP, const std::string& current_path, const std::string& filename)
{
	fs::path cp(current_path);

	if ( !fs::exists( cp ) )
	{
		Engine::out(Engine::ERROR) << "[IO] Unable to load path! '" << cp.generic_string() << "' not found!" << std::endl;
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
			Engine::out(Engine::ERROR) << "[IO] Unable to load directory '" << cp.generic_string() << "' into a ptree!" << std::endl;
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
		Engine::out(Engine::ERROR) << "[IO] Unable to load path! '" << cp.generic_string() << "' not a directory nor a file!" << std::endl;
	}

	return std::vector<std::shared_ptr<T>>();
}

template<class T>
std::vector<std::shared_ptr<T>> IO::loadFile( std::shared_ptr<IOPlugin>& IOP, const fs::path& p )
{
	auto ti = std::type_index( typeid(T) );

	auto vec = IOP->getSupportedFileExtensions();
	// get extension and remove the . from .ext
	std::string ext = p.extension().string().substr(1);
	if (std::find(vec.begin(), vec.end(), ext) == vec.end() )
	{
		Engine::out(Engine::ERROR) << "[IO] Unsupported file extension '" << ext << "' for '" << ti.name() << "'!" << std::endl;
		return std::vector<std::shared_ptr<T>>();
	}

	Engine::out() << "[IO] Loading " << p << " (" << ti.name() << ") ... " ;

	fs::ifstream in(p);

	if (IOP->loader_type == IOPlugin::loader::BINARY )
	{

			auto BinIO = dynamic_pointer_cast<iBinaryIOPlugin<T>>(IOP);
			auto re = BinIO->decodeStream ( in );
			Engine::out() << "done!" << std::endl;
			return re;
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

		Engine::out() << "done! ( got " << re.size() << " objs )" << std::endl;

		return re;
	}
}

template <class T>
std::vector<std::shared_ptr<T> > IO::loadObjects( const std::string name ){
	auto ti = std::type_index( typeid(T) );

	std::shared_ptr<IOPlugin> iop = getPlugin( ti );
	if(!iop) return std::vector<std::shared_ptr<T> >();

	// determine filename
	std::string filename = name;
	if ( filename.find('.') == std::string::npos )
		filename += '.' + iop->getSupportedFileExtensions()[0];

	return loadPath<T>(filename);

}

template <class T>
std::vector<std::shared_ptr<T> > IO::loadObjects(){
	std::shared_ptr<IOPlugin> iop = getPlugin( std::type_index( typeid(T) ) );
	if(!iop) return std::vector<std::shared_ptr<T> >();

	return loadPath<T>(iop->relative_path);
}

// #################################################
//						SAVING
// #################################################

template<class T>
bool IO::saveObject( const std::string& name, const T& object, bool overwrite )
{
	auto ti = std::type_index( typeid(T) );

	std::shared_ptr<IOPlugin> IOP = getPlugin( ti );
	if ( !IOP ) return false;

	try {
		auto tmp = getOfstream( IOP, name, Paths.front(), overwrite );
		if (!tmp.first->is_open()) return false;

		if ( saveFile(IOP, object, name, *(tmp.first)) )
		{
			Engine::out() << "[IO] Saved " << name << " ( " << tmp.second  << " -- " << ti.name() << ")" << std::endl;
		}

		tmp.first->close();
		return true;
	}
	catch (fs::filesystem_error& e)
	{
		Engine::out(Engine::ERROR) << "[IO] boost::fs exception! '" << e.what() << "'" << std::endl;
	}

	return false;
}

template <class T>
bool IO::saveFile( std::shared_ptr<IOPlugin>& IOP, const T& object, const std::string& name, fs::ofstream& out)
{
	if (IOP->loader_type == IOPlugin::loader::BINARY )
	{
			auto BinIO = dynamic_pointer_cast<iBinaryIOPlugin<T>>(IOP);
			return BinIO->encodeStream ( object, out );
	}
	else
	{
		auto TreeIO = dynamic_pointer_cast<iTreeIOPlugin<T>>(IOP);

		pt::ptree tree;

		if (TreeIO->saveObject( name, object, tree))
		{
			pt::info_parser::write_info( out, tree );
			return true;
		}

		Engine::out(Engine::ERROR) << "[IO] Error creating ptree for: " << name << " ( " << typeid(T).name() << ")" << std::endl;

		return false;
	}
}

template <class T, class Iter>
bool IO::saveObjects(Iter start, Iter end, bool ov){
	auto ti = std::type_index( typeid(T) );
	std::shared_ptr<IOPlugin> IOP = getPlugin( ti );
	if ( !IOP ) return false;

	if( IOP->loader_type == IOPlugin::loader::PTREE) {
		auto tio = dynamic_pointer_cast<iTreeIOPlugin<T>>(IOP);
		pt::ptree tree;

		for( auto it = start; it != end; ++it){
			if(tio->saveObject(ti.name(), **it, tree)){ // need some real name here instead of "some"... grrr.
				Engine::out(Engine::SPAM) << "[IO] Saved " << ti.name() << std::endl;
			} else {
				Engine::out(Engine::ERROR) << "[IO] Error creating ptree for: " << ti.name() << std::endl;
				return false;
			}
		}

		try {
			auto tmp = getOfstream( IOP, IOP->relative_path, Paths.front(), ov);
			if(!tmp.first->is_open()) return false;
			// write the tree:
			pt::info_parser::write_info( *(tmp.first), tree);

			tmp.first->close();
		}
		catch ( fs::filesystem_error &e) {
			Engine::out(Engine::ERROR) << "[IO] boost::fs exception! '" << e.what() << "'" << std::endl;
		}
		return true;
	} else{
		return false; // binary not supported...
	}
}

template<class T>
bool IO::saveObjects( std::map<std::string,std::shared_ptr<T>>& Objects, bool overwrite )
{
	auto ti = std::type_index( typeid(T) );
	std::shared_ptr<IOPlugin> IOP = getPlugin( ti );
	if ( !IOP ) return false;


	if ( IOP->loader_type == IOPlugin::loader::BINARY)
	{
		bool re = true;
		for ( auto it = Objects.begin() ; it != Objects.end(); ++it )
		{

			if ( !saveObject( it->first, *(it->second), overwrite) ) re = false;
		}

		return re;
	}
	else
	{


		auto TreeIO = dynamic_pointer_cast<iTreeIOPlugin<T>>(IOP);
		pt::ptree tree;

		for ( auto it = Objects.begin() ; it != Objects.end(); ++it )
		{
			const std::string& name = it->first;

			if ( TreeIO->saveObject( name, *it->second, tree))
			{
				Engine::out(Engine::SPAM) << "[IO] Saved " << name << " ( " << ti.name() << " )" << std::endl;
			}
			else
			{
				Engine::out(Engine::ERROR) << "[IO] Error creating ptree for: " << name << " ( " << ti.name() << " )" << std::endl;
				return false;
			}
		}

		try {

			auto tmp = getOfstream( IOP, IOP->relative_path, Paths.front(), overwrite );
			if (!tmp.first->is_open()) return false;
			// write the complete ptree
			pt::info_parser::write_info( *(tmp.first), tree );

			tmp.first->close();
		}
		catch (fs::filesystem_error& e)
		{
			Engine::out(Engine::ERROR) << "[IO] boost::fs exception! '" << e.what() << "'" << std::endl;
		}

		return true;
	}

}
