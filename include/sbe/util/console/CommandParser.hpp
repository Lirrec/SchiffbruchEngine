#ifndef COMMAND_PARSER
#define COMMAND_PARSER

#include <string>
#include <memory>
#include <vector>
#include "Node.hpp"

namespace sbe
{
	/**

	*/
	class CommandParser
	{
		public:

			CommandParser();
			~CommandParser() {}

			void        Execute(  std::string s );
			std::string Complete( std::string s );

		private:
			void MakeCommandTree();
			void switchToNextPossibility();
			void interpretNew();
			std::vector<std::string> split( std::string s, char c );
			std::string recievedCommand;
			std::string sentCommand;
			std::shared_ptr<Node> currentNode;
			std::list<std::shared_ptr<Node>> Possibilities;
			bool commandNotFound;
			std::vector<std::string> CommandVec;
			///CommandTree
			std::shared_ptr<Node> ct;
	};
} // namespace sbe
#endif // COMMAND_PARSER
