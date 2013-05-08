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

			///Tries to execute the given Command
			void Execute( std::string s );

			///return the last of history
			std::string FetchHistory( std::string s, int x );

			///Tries to complete the given Command
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

			///root of CommandTree
			std::shared_ptr<Node> ct;

			///history of successfully executed commands
			std::list<std::string> history;

			///accessiterator of history
			std::list<std::string>::iterator historyAccess;
	};
} // namespace sbe
#endif // COMMAND_PARSER
