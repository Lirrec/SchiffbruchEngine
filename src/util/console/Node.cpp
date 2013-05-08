#include "sbe/util/console/Node.hpp"

namespace sbe
{
	Node::Node( std::string command_name )
	: name( command_name )
	{

	}

	std::list<std::shared_ptr<Node>> Node::GetAll( std::string s )
	{
		std::list<std::shared_ptr<Node>> results;
		for ( auto it = subNodes.begin(); it != subNodes.end(); it++ )
		{
			if ( ( *it )->beginsWith( s ) )
				results.push_back( *it );
		}
		return results;
	}

	bool Node::Is( std::string s )
	{
		return ( name.compare( s ) == 0 ? true : false );
	}

	bool Node::HasWith( std::string s )
	{
		bool r = false;
		for ( auto it = subNodes.begin(); it != subNodes.end(); it++ )
		{
			if ( ( *it )->Name().compare( 0, s.length(), s ) )
				r = true;
		}
		return r;
	}
	std::shared_ptr<Node> Node::Get( std::string s )
	{
		for ( auto it = subNodes.begin(); it != subNodes.end(); it++ )
		{
			if ( ( *it )->name == s )
				return *it;
		}
		return nullptr;
	}

	std::string Node::Name()
	{
		return name;
	}

	std::string Node::Help()
	{
		return help;
	}

	void Node::AddSub( std::shared_ptr<Node> sub )
	{
		subNodes.push_back( sub );
	}

	bool Node::beginsWith( std::string s )
	{
		return !name.compare( 0, s.length(), s );
	}
}
