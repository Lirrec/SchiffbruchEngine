#include "sbe/util/console/CommandParser.hpp"

#include "sbe/Engine.hpp"
#include "sbe/Module.hpp"
#include "sbe/util/console/CommandNode.hpp"

using namespace std;

namespace sbe
{
	CommandParser::CommandParser()
	{
		ct.reset( new CommandNode( "/" ) );
		/******/ct->AddSub( shared_ptr<Node>( new CommandNode( "conf" ) ) );
		/******/ct->AddSub( shared_ptr<Node>( new CommandNode( "hide" ) ) );
		/******/ct->Get( "hide" )->AddSub( shared_ptr<Node>( new CommandNode( "alloverlays" ) ) );
		/******/ct->Get( "hide" )->AddSub( shared_ptr<Node>( new CommandNode( "overlay" ) ) );
		/******/ct->Get( "hide" )->AddSub( shared_ptr<Node>( new CommandNode( "window" ) ) );
		/******/ct->AddSub( shared_ptr<Node>( new CommandNode( "kill" ) ) );
		/******/ct->Get( "kill" )->AddSub( shared_ptr<Node>( new CommandNode( "carnivore" ) ) );
		/******/ct->Get( "kill" )->AddSub( shared_ptr<Node>( new CommandNode( "herbivore" ) ) );
		/******/ct->Get( "kill" )->AddSub( shared_ptr<Node>( new CommandNode( "plant" ) ) );
		/******/ct->AddSub( shared_ptr<Node>( new CommandNode( "show" ) ) );
		/******/ct->Get( "show" )->AddSub( shared_ptr<Node>( new CommandNode( "overlay" ) ) );
		/******/ct->Get( "show" )->AddSub( shared_ptr<Node>( new CommandNode( "window" ) ) );
		/******/ct->AddSub( shared_ptr<Node>( new CommandNode( "sim" ) ) );
		/******/ct->Get( "sim" )->AddSub( shared_ptr<Node>( new CommandNode( "restart" ) ) );
		/******/ct->Get( "sim" )->AddSub( shared_ptr<Node>( new CommandNode( "run" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->AddSub( shared_ptr<Node>( new CommandNode( "ticks" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->AddSub( shared_ptr<Node>( new CommandNode( "untildeath" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->Get( "untildeath" )->AddSub( shared_ptr<Node>( new CommandNode( "carnivore" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->Get( "untildeath" )->AddSub( shared_ptr<Node>( new CommandNode( "herbivore" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->Get( "untildeath" )->AddSub( shared_ptr<Node>( new CommandNode( "plant" ) ) );
		/******/ct->Get( "sim" )->AddSub( shared_ptr<Node>( new CommandNode( "stop" ) ) );
		/******/ct->Get( "sim" )->Get( "stop" )->AddSub( shared_ptr<Node>( new CommandNode( "ontick" ) ) );
		sentCommand = "";
		recievedCommand = "";
		currentNode = ct;
		commandNotFound = false;
	}

	void CommandParser::Execute( string s )
	{
	///@TODO
	/******/	if ( s == "pause" )
	/******/		{ Module::Get()->QueueEvent( "KEY_SIM_PAUSE", true ); }
	/******/	else if ( s == "graph" )
	/******/		{ Module::Get()->QueueEvent( "KEY_SHOW_GRAPHBOOK", true ); }
	/******/	else
	/******/		{ Engine::out() << "[CommandParser] command not found." << endl; }
	}

	string CommandParser::Complete( string s )
	{
		if ( s == sentCommand && !commandNotFound )
		{
			switchToNextPossibility();
		}
		else
		{
			commandNotFound = false;
			recievedCommand = s;
			interpretNew();
		}
		if ( commandNotFound )
		{
			Engine::out() << "[CommandParser] command not found." << endl;
		}

		return sentCommand;
	}

	void CommandParser::interpretNew()
	{
		CommandVec = split( recievedCommand, ' ' );
		currentNode = ct;
		for ( auto it = CommandVec.begin(); it != CommandVec.end() - 1; it++ )
		{
			if ( currentNode->Get( *it ) )
			{
				currentNode = currentNode->Get( *it );
			}
			else
			{
				commandNotFound = true;
				sentCommand = recievedCommand;
				return;
			}
		}

		Possibilities = currentNode->GetAll( CommandVec.back() );
		sentCommand = "";
		for ( auto it = CommandVec.begin(); it != CommandVec.end() - 1; it++ )
		{
			sentCommand += *it;
			sentCommand += " ";
		}
		if ( !Possibilities.size() )
		{
			commandNotFound = true;
			sentCommand = recievedCommand;
			return;
		}
		else
		{
			sentCommand += Possibilities.front()->Name();
		}
		return;
	}

	void CommandParser::switchToNextPossibility()
	{
		string topic = split( sentCommand, ' ' ).back();
		sentCommand = sentCommand.substr( 0, sentCommand.length() - topic.length() );
		auto it = Possibilities.begin();
		for ( ; it != --(Possibilities.end()); it++ )
		{
			if ( ( *it )->Is( topic ) )
			{
				sentCommand += ( *( ++it ) )->Name();
				return;
			}
		}
		sentCommand += Possibilities.front()->Name();
		return;
	}

	vector<string> CommandParser::split( string s, char c )
	{
		vector<string> results;
		int mark = 0;
		for ( int i = 0; i < s.size(); i++ )
		{
			if ( s.at( i ) == c )
			{
				results.push_back( s.substr( mark, i - mark ) );
				mark = i + 1;
			}
		}
		results.push_back( s.substr( mark, s.length() - mark ) );
		return results;
	}

//	void CommandParser::AddNode( shared_ptr<Node> )
//	{

//	}
}
