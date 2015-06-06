#include "sbe/util/console/CommandNode.hpp"

#include "sbe/Module.hpp"

namespace sbe
{
	CommandNode::CommandNode(std::string command_name, std::string command_event)
			: Node(command_name),
			  event(command_event) {

	}

	void CommandNode::Execute(std::vector<std::string> Parameters) {
		Module::Get()->QueueEvent(event, true);
	}

}
