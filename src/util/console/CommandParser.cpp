#include "sbe/util/console/CommandParser.hpp"

#include "sbe/Engine.hpp"
#include "sbe/Module.hpp"
#include "sbe/util/console/CommandNode.hpp"
//#include "sbe/util/console/ArgumentsCommandNode.hpp"

#include <algorithm>

namespace sbe
{
	CommandParser::CommandParser()
	{
		ct.reset( new Node( "/" ) );
		/******/ct->AddSub( std::shared_ptr<Node>( new Node( "conf" ) ) );
		/******/ct->Get( "conf" )->AddSub( std::shared_ptr<Node>( new Node( "set" ) ) );
		/******/ct->AddSub( std::shared_ptr<Node>( new Node( "debug" ) ) );
		/******/ct->Get( "debug" )->AddSub( std::shared_ptr<Node>( new CommandNode( "reload_graph", "debug_reload_graph" ) ) );
		/******/ct->AddSub( std::shared_ptr<Node>( new Node( "hide" ) ) );
		/******/ct->Get( "hide" )->AddSub( std::shared_ptr<Node>( new CommandNode( "alloverlays", "EVT_COMMAND_HIDE_ALL_OVERLAYS" ) ) );
		/******/ct->Get( "hide" )->AddSub( std::shared_ptr<Node>( new Node( "overlay" ) ) );
		/******/ct->Get( "hide" )->AddSub( std::shared_ptr<Node>( new CommandNode( "window", "EVT_COMMAND_HIDE_WINDOW" ) ) );
		/******/ct->AddSub( std::shared_ptr<Node>( new Node( "kill" ) ) );
		/******/ct->Get( "kill" )->AddSub( std::shared_ptr<Node>( new Node( "carnivore" ) ) );
		/******/ct->Get( "kill" )->AddSub( std::shared_ptr<Node>( new Node( "herbivore" ) ) );
		/******/ct->Get( "kill" )->AddSub( std::shared_ptr<Node>( new Node( "plant" ) ) );
		/******/ct->AddSub( std::shared_ptr<Node>( new Node( "show" ) ) );
		/******/ct->Get( "show" )->AddSub( std::shared_ptr<Node>( new Node( "overlay" ) ) );
		/******/ct->Get( "show" )->AddSub( std::shared_ptr<Node>( new Node( "window" ) ) );
		/******/ct->AddSub( std::shared_ptr<Node>( new Node( "sim" ) ) );
		/******/ct->Get( "sim" )->AddSub( std::shared_ptr<Node>( new Node( "restart" ) ) );
		/******/ct->Get( "sim" )->AddSub( std::shared_ptr<Node>( new CommandNode( "run", "KEY_SIM_PAUSE" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->AddSub( std::shared_ptr<Node>( new Node( "ticks" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->AddSub( std::shared_ptr<Node>( new Node( "untildeathof" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->Get( "untildeathof" )->AddSub( std::shared_ptr<Node>( new Node( "carnivore" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->Get( "untildeathof" )->AddSub( std::shared_ptr<Node>( new Node( "herbivore" ) ) );
		/******/ct->Get( "sim" )->Get( "run" )->Get( "untildeathof" )->AddSub( std::shared_ptr<Node>( new Node( "plant" ) ) );
		/******/ct->Get( "sim" )->AddSub( std::shared_ptr<Node>( new Node( "stop" ) ) );
		/******/ct->Get( "sim" )->Get( "stop" )->AddSub( std::shared_ptr<Node>( new Node( "ontick" ) ) );
		/******/ct->AddSub( std::shared_ptr<Node>( new Node( "toggle" ) ) );
		/******/ct->Get( "toggle" )->AddSub( std::shared_ptr<Node>( new Node( "window" ) ) );
		/******/ct->Get( "toggle" )->Get( "window" )->AddSub( std::shared_ptr<Node>( new CommandNode( "console", "KEY_SHOW_CONSOLE" ) ) );
		/******/ct->Get( "toggle" )->Get( "window" )->AddSub( std::shared_ptr<Node>( new CommandNode( "infopanel", "KEY_SHOW_INFOPANEL" ) ) );
		/******/ct->Get( "toggle" )->Get( "window" )->AddSub( std::shared_ptr<Node>( new CommandNode( "graphbook", "KEY_SHOW_GRAPHBOOK" ) ) );
		/******/ct->Get( "toggle" )->Get( "window" )->AddSub( std::shared_ptr<Node>( new CommandNode( "mainmenu", "KEY_SHOW_MAINMENU" ) ) );

		historyAccess = history.end();
	}

	void CommandParser::Execute( const std::string& s )
	{
		auto cmd = parseCommandString( s );


		if ( !history.empty() && history.back() != s )
			history.push_back( s );
		//reset history-access
		historyAccess = history.end();

		if ( !cmd.valid )
		{
			if ( cmd.CommandVec.back() != "" )
			{
				Engine::out( Engine::INFO ) << "Command '" << cmd.ToString() << "' not found!" << std::endl;
				return;
			}
		}

		std::vector<std::string> Params;

		//while ( cmd.currentNode)
		///TODO: parse params

		if (  cmd.currentNode->IsExecutable() )
		{
			cmd.currentNode->Execute( Params );
			Engine::out( Engine::INFO ) << s << std::endl;
		}
	}

	std::string CommandParser::Complete( const std::string& s )
	{
		//reset history-access
		historyAccess = history.end();

		auto cmd = parseCommandString( s );
		cmd.NextCompletion();

		return cmd.ToString();
	}


	std::string CommandParser::FetchHistory( const std::string& s, int x )
	{
		if ( x==0 || history.size() == 0 )
			return s;

		if ( s == "" )
			historyAccess = history.end();

		// back in time
		if ( x > 0 )
		{
			if ( historyAccess != history.begin() ) --historyAccess;
		}
		// back to the future!
		else if ( x < 0 )
		{
			if ( historyAccess != history.end() ) historyAccess++;
		}

		return historyAccess == history.end() ? "" : *historyAccess;
	}

	CommandParser::Command CommandParser::parseCommandString( const std::string& receivedCommand )
	{
		Command C;
		C.CommandVec = split( receivedCommand, ' ' );

		C.currentNode = ct;
		for ( auto it = C.CommandVec.begin(); it != C.CommandVec.end(); it++ )
		{
			if ( C.currentNode->Get( *it ) )
				C.currentNode = C.currentNode->Get( *it );
			else
			{
				C.valid = false;
				// remove trailing elements
				if ( (it+1) != C.CommandVec.end() ) C.CommandVec.erase( it+1, C.CommandVec.end() );
				break;
			}
		}

		//Engine::out( Engine::INFO ) << "valid? " << C.valid << std::endl;
		//for ( auto s : C.CommandVec) Engine::out(Engine::INFO) << s << std::endl;
		//Engine::out( Engine::INFO ) << "Currentnode: " << C.currentNode->Name() << std::endl;

		return C;
	}

	void CommandParser::Command::NextCompletion()
	{
		std::list<std::shared_ptr<Node>> PossibleCompletions;

		if ( CommandVec.empty() ) return;
		std::string& lastpart = CommandVec.back();

		if ( valid )
		{
			PossibleCompletions = currentNode->Parent()->GetAll();
		}
		else
		{
			PossibleCompletions = currentNode->GetAll( lastpart );
			if ( PossibleCompletions.empty()) PossibleCompletions = currentNode->GetAll();
		}


		if ( PossibleCompletions.empty() )
		{
			Engine::out( Engine::INFO ) << "No completion found!" << std::endl;
			return;
		}

		//for ( auto& Node : PossibleCompletions ) Engine::out(Engine::INFO) << Node->Name() << std::endl;
		//Engine::out(Engine::INFO) << "lastpart: '" << lastpart << "'" << std::endl;

		auto it = std::find_if(PossibleCompletions.begin(), PossibleCompletions.end(), [lastpart](std::shared_ptr<Node>& n){ return n->Is(lastpart); } );

		if ( it == PossibleCompletions.end() )
		{
			lastpart = PossibleCompletions.front()->Name();
			//Engine::out(Engine::INFO) << ": '" << lastpart << "'" << std::endl;
			return;
		}

		auto next = ++it;
		--it;
		if ( next == PossibleCompletions.end())
			lastpart = PossibleCompletions.front()->Name();
		else
			lastpart = (*next)->Name();
	}

	std::string CommandParser::Command::ToString()
	{
		std::string re;

		for ( auto s : CommandVec ) {
			re += s;
			re += ' ';
		}

		if (re.back() == ' ') re.pop_back();

		return re;
	}

	std::vector<std::string> CommandParser::split( const std::string& s, char c )
	{
		std::vector<std::string> results;
		int mark = 0;
		for ( size_t i = 0; i < s.size(); i++ )
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
