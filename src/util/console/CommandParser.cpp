#include "sbe/util/console/CommandParser.hpp"

#include "sbe/Engine.hpp"
#include "sbe/Module.hpp"
#include "sbe/util/console/CommandNode.hpp"
#include "sbe/util/console/ArgumentsCommandNode.hpp"

using namespace std;

namespace sbe
{
	CommandParser::CommandParser()
	{
		ct.reset( new Node( "/" ) );
		/******/ct->AddSub( shared_ptr<Node>( new Node( "conf" ) ) );
		/******/ct->Get( "conf" )->AddSub( shared_ptr<Node>( new Node( "set" ) ) );
		/******/ct->AddSub( shared_ptr<Node>( new Node( "hide" ) ) );
		/******/ct->Get( "hide" )->AddSub( shared_ptr<Node>( new CommandNode( "alloverlays", "EVT_COMMAND_HIDE_ALL_OVERLAYS" ) ) );
		/******/ct->Get( "hide" )->AddSub( shared_ptr<Node>( new Node( "overlay" ) ) );
		/******/ct->Get( "hide" )->AddSub( shared_ptr<Node>( new ArgumentsCommandNode( "window", "EVT_COMMAND_HIDE_WINDOW" ) ) );
		/******/ct->AddSub( shared_ptr<Node>( new Node( "kill" ) ) );
		/******/ct->Get( "kill" )->AddSub( shared_ptr<Node>( new Node( "carnivore" ) ) );
		/******/ct->Get( "kill" )->AddSub( shared_ptr<Node>( new Node( "herbivore" ) ) );
		/******/ct->Get( "kill" )->AddSub( shared_ptr<Node>( new Node( "plant" ) ) );
		/******/ct->AddSub( shared_ptr<Node>( new Node( "show" ) ) );
		/******/ct->Get( "show" )->AddSub( shared_ptr<Node>( new Node( "overlay" ) ) );
		/******/ct->Get( "show" )->AddSub( shared_ptr<Node>( new Node( "window" ) ) );
		/******/ct->AddSub( shared_ptr<Node>( new Node( "sim" ) ) );
		/******/ct->Get( "sim" )->AddSub( shared_ptr<Node>( new Node( "restart" ) ) );
		/******/ct->Get( "sim" )->AddSub( shared_ptr<Node>( new CommandNode( "run", "KEY_SIM_PAUSE" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->AddSub( shared_ptr<Node>( new Node( "ticks" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->AddSub( shared_ptr<Node>( new Node( "untildeathof" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->Get( "untildeathof" )->AddSub( shared_ptr<Node>( new Node( "carnivore" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->Get( "untildeathof" )->AddSub( shared_ptr<Node>( new Node( "herbivore" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->Get( "untildeathof" )->AddSub( shared_ptr<Node>( new Node( "plant" ) ) );
		/******/ct->Get( "sim" )->AddSub( shared_ptr<Node>( new Node( "stop" ) ) );
		/******/ct->Get( "sim" )->Get( "stop" )->AddSub( shared_ptr<Node>( new Node( "ontick" ) ) );
		/******/ct->AddSub( shared_ptr<Node>( new Node( "toggle" ) ) );
		/******/ct->Get( "toggle" )->AddSub( shared_ptr<Node>( new Node( "window" ) ) );
		/******/ct->Get( "toggle" )->Get( "window" )->AddSub( shared_ptr<Node>( new CommandNode( "console", "KEY_SHOW_CONSOLE" ) ) );
		/******/ct->Get( "toggle" )->Get( "window" )->AddSub( shared_ptr<Node>( new CommandNode( "infopanel", "KEY_SHOW_INFOPANEL" ) ) );
		/******/ct->Get( "toggle" )->Get( "window" )->AddSub( shared_ptr<Node>( new CommandNode( "graphbook", "KEY_SHOW_GRAPHBOOK" ) ) );
		/******/ct->Get( "toggle" )->Get( "window" )->AddSub( shared_ptr<Node>( new CommandNode( "mainmenu", "KEY_SHOW_MAINMENU" ) ) );
		sentCommand = "";
		recievedCommand = "";
		currentNode = ct;
		commandNotFound = false;
		historyAccess = history.end();
	}

	void CommandParser::Execute( string s )
	{
		///@TODO
		/******/	if ( s == "pause" )
		/******/		{ Module::Get()->QueueEvent( "KEY_SIM_PAUSE", true ); }
		/******/	else if ( s == "graph" )
		/******/		{ Module::Get()->QueueEvent( "KEY_SHOW_GRAPHBOOK", true ); }

		recievedCommand = s;
//		//mkhistory
//		auto it = history.begin();
//		for ( ; it != history.end(); it++ )
//		{
//			if ( *it == recievedCommand )
//				break;
//		}
//		if ( it != history.end() )
//			history.erase( it );
		history.push_back( recievedCommand );
		//reset history-access
		historyAccess = history.end();
		//execute
		CommandVec = split( recievedCommand, ' ' );
		currentNode = ct;
		for ( auto it = CommandVec.begin(); it != CommandVec.end(); it++ )
		{
			if ( currentNode->Get( *it ) )
				currentNode = currentNode->Get( *it );
			else
			{
				commandNotFound = true;
				sentCommand = "";
				recievedCommand = "";
				Engine::out( Engine::INFO ) << "Command not found, no execution possible!" << endl;
				return;
			}
		}
		if ( currentNode->IsExecutable() )
		{
			shared_ptr<CommandNode> x = dynamic_pointer_cast<CommandNode> ( currentNode );
			Module::Get()->QueueEvent( x->Event(), true );
			Engine::out( Engine::INFO ) << recievedCommand << endl;
		}
	}

	string CommandParser::FetchHistory( string s, int x )
	{
		if (( sentCommand != s && s != "" ) || ( x == 0 ) || (history.size() == 0 ))
			return s;
		if ( s == "" )
			historyAccess = history.end();
		sentCommand = "";
		// UP  the first try
		if      ( x > 0 && historyAccess == history.end() )
			sentCommand = *(--historyAccess);
		// UP  the last one is already called (will come again)
		else if ( x > 0 && historyAccess == history.begin() )
			sentCommand = *historyAccess;
		// UP this is the last one
		else if ( x > 0 && --historyAccess == history.begin() )
			sentCommand = *historyAccess;
		// UP else
		else if ( x > 0 )
			sentCommand = *historyAccess; //no -- because allready in if one above
		// DOWN the first try
		else if ( x < 0 && historyAccess == history.end() )
			{}
		// DOWN return to end
		else if ( x < 0 && ++historyAccess == history.end() )
			{}
		// DOWN everytimes else
		else if ( x < 0 )
			sentCommand = *(historyAccess); //no ++ because already in if one above done
		return sentCommand;
	}

	string CommandParser::Complete( string s )
	{
		//reset history-access
		historyAccess = history.end();
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
			Engine::out( Engine::INFO ) << "Command not found, no completion possible!" << endl;
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
