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

		~CommandParser() { }

		///Tries to execute the given Command
		void Execute(const std::string& s);

		///return the last of history
		std::string FetchHistory(const std::string& s, int x);

		///Tries to complete the given Command
		std::string Complete(const std::string& s);

	private:

		class Command
		{
		public:
			std::vector<std::string> CommandVec;
			std::shared_ptr<Node> currentNode;
			bool valid = true;

			void NextCompletion();

			std::string ToString();
		};

		void MakeCommandTree();


		/**
			Parses a string into a command object
		*/
		Command parseCommandString(const std::string& receivedCommand);

		std::vector<std::string> split(const std::string& s, char c);

		///root of CommandTree
		std::shared_ptr<Node> ct;

		///history of successfully executed commands
		std::list<std::string> history;

		///accessiterator of history
		std::list<std::string>::iterator historyAccess;
	};
} // namespace sbe
#endif // COMMAND_PARSER
