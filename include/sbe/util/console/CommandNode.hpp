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

			CommandNode( std::string command_name );
			~CommandNode() {}



		protected:

	};
} // namespace sbe
#endif // COMMAND_NODE
