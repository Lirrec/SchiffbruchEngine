#ifndef COMMAND_NODE
#define COMMAND_NODE

#include "Node.hpp"
#include <string>

namespace sbe
{
	/**

	*/
	class CommandNode : public Node
	{
	public:

		CommandNode(std::string command_name, std::string command_event);

		~CommandNode() { }

		///returns the name of the event-string
		void Execute(std::vector<std::string> Parameters) override;

		///returns true if event is not empty
		bool IsExecutable() override { return event.length() != 0; }

	protected:

		///contains the event-string
		std::string event;

	};
} // namespace sbe
#endif // COMMAND_NODE
