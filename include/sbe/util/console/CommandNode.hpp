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

			CommandNode( std::string command_name, std::string command_event );
			~CommandNode() {}

			///returns the name of the event-string
			virtual std::string Event();

			///returns true if event is not empty
			virtual bool IsExecutable() { return event.length() != 0; }

		protected:

			///contains the event-string
			std::string event;

	};
} // namespace sbe
#endif // COMMAND_NODE
