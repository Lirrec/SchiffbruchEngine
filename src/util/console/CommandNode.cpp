#include "sbe/util/console/CommandNode.hpp"

namespace sbe
{
	CommandNode::CommandNode( std::string command_name, std::string command_event )
	: Node( command_name ),
	  event( command_event )
	{

	}

	std::string CommandNode::Event()
	{
		return event;
	}

}
