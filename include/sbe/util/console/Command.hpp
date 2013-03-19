#ifndef COMMAND
#define COMMAND

#include "Node.hpp"
#include <string>
namespace sbe
{
	/**

	*/
	class Command : public Node
	{
		public:

			Command();
			~Command() {}

		private:
			std::string CommandName;
	};
} // namespace sbe
#endif // COMMAND
