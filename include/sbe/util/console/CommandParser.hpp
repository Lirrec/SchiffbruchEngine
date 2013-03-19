#ifndef COMMAND_PARSER
#define COMMAND_PARSER

#include <string>

namespace sbe
{
	/**

	*/
	class CommandParser
	{
		public:

			CommandParser();
			~CommandParser() {}

			void Execute( std::string c );
			std::string Complete( std::string c );
		private:
			std::string CommandString;
	};
} // namespace sbe
#endif // COMMAND_PARSER
