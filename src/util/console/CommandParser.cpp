#include "sbe/util/console/CommandParser.hpp"

#include "sbe/Module.hpp"

namespace sbe
{
	CommandParser::CommandParser()
	{
		CommandString = "";
	}

	void CommandParser::Execute( std::string c )
	{
		CommandString = c;
		if ( CommandString == "pause" )
		{
			Module::Get()->QueueEvent( "KEY_SIM_PAUSE", true );
		}
		else if ( CommandString == "graph" )
		{
			Module::Get()->QueueEvent( "KEY_SHOW_GRAPHBOOK", true );
		}
		else
		{
			Engine::out() << "[CommandParser] command not found." << std::endl;
		}
	}

	std::string CommandParser::Complete( std::string c )
	{
		///@TODO: search in tree (first implement tree)
		return c + " tab";
	}
}
