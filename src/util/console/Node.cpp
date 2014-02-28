#include "sbe/util/console/Node.hpp"

#include "sbe/Engine.hpp"
#include <iostream>

namespace sbe
{
	Node::Node( const std::string& command_name )
	: name( command_name ), parent( nullptr )
	{

	}

	std::list<std::shared_ptr<Node>> Node::GetAll( const std::string& s )
	{
		std::list<std::shared_ptr<Node>> results;

		if ( s == "" ) return subNodes;

		for ( std::shared_ptr<Node>& N : subNodes )
			if ( beginsWith(N->Name(), s) )
				results.push_back( N );

		return results;
	}

	bool Node::Is( const std::string& s )
	{
		return name == s;
	}

	bool Node::HasWith( const std::string& s )
	{
		for ( std::shared_ptr<Node>& N : subNodes )
			if ( beginsWith(N->Name(), s) )
				return true;

		return false;
	}
	std::shared_ptr<Node> Node::Get( const std::string& s )
	{
		for ( std::shared_ptr<Node>& N : subNodes )
			if ( N->Name() == s )
				return N;

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
		sub->setParent( this );
	}

	bool Node::beginsWith( const std::string& s, const std::string& prefix )
	{
		return !s.compare( 0, prefix.length(), prefix );
	}
}
