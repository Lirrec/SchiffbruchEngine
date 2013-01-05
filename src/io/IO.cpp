#include "sbe/io/IO.hpp"

IO::IO()
{
	// add default resources directory ( "res" subdirectory in executable dir )
	Paths.push_back( (fs::current_path() / "res").generic_string() );
}

IO::~IO()
{

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

std::shared_ptr<IOPlugin> IO::getPlugin( const std::type_info& ti )
{
	return getPlugin( std::type_index(ti));
}

std::shared_ptr<IOPlugin> IO::getPlugin( const std::type_index& ti )
{
	if ( Plugins.find( ti ) == Plugins.end() )
	{
		Engine::out() << "[IO] No IOPlugin found for type '" << ti.name() << "'!" << std::endl;
		return std::shared_ptr<IOPlugin>();
	}

	return Plugins[ti];
}


std::pair<std::shared_ptr<fs::ofstream>, std::string> IO::getOfstream( std::shared_ptr<IOPlugin>& IOP, const std::string& name, const std::string& path, bool overwrite )
{

	std::shared_ptr<fs::ofstream> re( new fs::ofstream() );

	// determine filename
	std::string filename = name;
	if ( filename.find('.') == std::string::npos )
		filename += '.' + IOP->getSupportedFileExtensions()[0];

	fs::path p ( path );

	p /= filename;

	if ( fs::status(p).type() == fs::file_type::file_not_found || (fs::is_regular_file(p) && overwrite) )
	{
		re->open(p);
	}
	else
	{
		Engine::out() << "[IO] Saving " << name << " ( " << p << " -- " << ") would overwrite existing file!" << std::endl;
	}

	return std::make_pair(re, p.string());
}
