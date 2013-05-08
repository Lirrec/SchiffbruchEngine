#ifndef NODE
#define NODE

#include <list>
#include <memory>
#include <string>

namespace sbe
{
	/**
	Class Node is used to create the command tree for autocompletion of console commands.
	The Tree is saved in /res/commandtree.conf and is build at the start of program by the CommandParser.
	*/
	class Node
	{
		public:

			Node( std::string command_name );
			virtual ~Node() {}

			///returns a list with subnodes beginning with a given string
			virtual std::list<std::shared_ptr<Node>> GetAll( std::string s );

			///returns true if given string is identic to node's command.
			virtual bool Is( std::string s );

			///overloaded by Commandnode which has an Event
			virtual bool IsExecutable() { return false; }

			///returns true if Node has subnode/s beginning with given string
			virtual bool HasWith( std::string s );

			///returns the child node with given command
			virtual std::shared_ptr<Node> Get( std::string s );

			///returns the name of the Node
			std::string Name();

			///returns the helptext
			std::string Help();

			///adds a subNode and sorts them alphabeticaly again
			virtual void AddSub( std::shared_ptr<Node> sub );

		protected:

			///returns true if name begins with given string
			bool beginsWith( std::string s );

			///contains the name of the node
			std::string name;

			///contains the helptext
			std::string help;

			///contains the list with subnodes
			std::list<std::shared_ptr<Node>> subNodes;

	};
} // namespace sbe
#endif // NODE
