#include "sbe/io/IO.hpp"

IO::IO()
{
	Paths.push_back(fs::current_path().generic_string());
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
