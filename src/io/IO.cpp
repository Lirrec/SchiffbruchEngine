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
